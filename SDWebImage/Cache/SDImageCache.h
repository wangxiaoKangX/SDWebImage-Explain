/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"
#import "SDImageCacheConfig.h"

typedef NS_ENUM(NSInteger, SDImageCacheType) {
    /**
*该图像不是SDWebImage缓存，而是从网络下载的。     */
    SDImageCacheTypeNone,
    /**
     * 图像是从磁盘缓存中获得的。
     */
    SDImageCacheTypeDisk,
    /**
     * 图像是从内存缓存中获得的。
     */
    SDImageCacheTypeMemory
};

typedef NS_OPTIONS(NSUInteger, SDImageCacheOptions) {
    /**
     * 默认情况下，当映像缓存到内存中时，我们不会查询磁盘数据。此掩码可以同时强制查询磁盘数据。
     */
    SDImageCacheQueryDataWhenInMemory = 1 << 0,
    /**
     * 默认情况下，我们同步查询内存缓存，异步地查询磁盘缓存。此掩码可以同步查询磁盘缓存。
     */
    SDImageCacheQueryDiskSync = 1 << 1,
    /**
     * 默认情况下，图像被解码，以尊重其原始大小。在iOS上，这面旗帜将会缩小。
     * 图像的大小与设备的受限内存兼容。
     */
    SDImageCacheScaleDownLargeImages = 1 << 2
};

typedef void(^SDCacheQueryCompletedBlock)(UIImage * _Nullable image, NSData * _Nullable data, SDImageCacheType cacheType);

typedef void(^SDWebImageCheckCacheCompletionBlock)(BOOL isInCache);

typedef void(^SDWebImageCalculateSizeBlock)(NSUInteger fileCount, NSUInteger totalSize);


/**
 * SDImageCache维护一个内存缓存和一个可选的磁盘缓存。执行磁盘缓存写入操作。
 * 异步，因此它不会给UI添加不必要的延迟。
 */
@interface SDImageCache : NSObject

#pragma mark - Properties

/**
 *  缓存配置对象-存储所有类型的设置。
 */
@property (nonatomic, nonnull, readonly) SDImageCacheConfig *config;

/**
 * 内存镜像缓存的最大“总成本”。成本函数是内存中的像素个数。
 */
@property (assign, nonatomic) NSUInteger maxMemoryCost;

/**
 * 缓存应该持有的对象的最大数量。
 */
@property (assign, nonatomic) NSUInteger maxMemoryCountLimit;

#pragma mark - Singleton and initialization

/**
 * 返回全局共享缓存实例。
 *
 * @return SDImageCache global instance
 */
+ (nonnull instancetype)sharedImageCache;

/**
 * Init一个具有特定名称空间的新缓存存储。
 *
 * @param ns The namespace to use for this cache store
 */
- (nonnull instancetype)initWithNamespace:(nonnull NSString *)ns;

/**
 * Init一个具有特定名称空间和目录的新缓存存储。
 *
 * @param ns        用于此缓存存储的名称空间。
 * @param directory Directory to cache disk images in
 */
- (nonnull instancetype)initWithNamespace:(nonnull NSString *)ns
                       diskCacheDirectory:(nonnull NSString *)directory NS_DESIGNATED_INITIALIZER;

#pragma mark - Cache paths

- (nullable NSString *)makeDiskCachePath:(nonnull NSString*)fullNamespace;

/**
 * 添加一个只读缓存路径，以搜索由SDImageCache预缓存的图像。
 *如果你想把预装的图片和你的应用捆绑在一起，这很有用。
 */
- (void)addReadOnlyCachePath:(nonnull NSString *)path;

#pragma mark - Store Ops

/**
 * 异步地将映像存储在给定键的内存和磁盘缓存中。.
 *
 * @param image           图像存储
 * @param key             唯一的图像缓存键，通常是图像绝对URL。
 * @param completionBlock在操作完成后执行的块。
 */
- (void)storeImage:(nullable UIImage *)image
            forKey:(nullable NSString *)key
        completion:(nullable SDWebImageNoParamsBlock)completionBlock;

/**
 * 异步地将映像存储在给定键的内存和磁盘缓存中。
 *
 * @param image           图像存储
 * @param key            唯一的图像缓存键，通常是图像绝对URL。
 * @param toDisk          如果是，将映像存储到磁盘缓存。
 * @param completionBlock在操作完成后执行的块。
 */
- (void)storeImage:(nullable UIImage *)image
            forKey:(nullable NSString *)key
            toDisk:(BOOL)toDisk
        completion:(nullable SDWebImageNoParamsBlock)completionBlock;

/**
 * 异步地将映像存储在给定键的内存和磁盘缓存中。
 */
- (void)storeImage:(nullable UIImage *)image
         imageData:(nullable NSData *)imageData
            forKey:(nullable NSString *)key
            toDisk:(BOOL)toDisk
        completion:(nullable SDWebImageNoParamsBlock)completionBlock;

/**
 * 同步地将图像NSData存储到给定密钥的磁盘缓存中。
 * @param key        唯一的图像缓存键，通常是图像绝对URL。
 */
- (void)storeImageDataToDisk:(nullable NSData *)imageData forKey:(nullable NSString *)key;

#pragma mark - Query and Retrieve Ops

/**
 *  异步检查磁盘缓存中是否存在图像(不加载图像)
 *
 *  @param key             描述url的键。
 *  @当检查完成时，param completionBlock将被执行。
 *  @注意，完成块将始终在主队列上执行。
 */
- (void)diskImageExistsWithKey:(nullable NSString *)key completion:(nullable SDWebImageCheckCacheCompletionBlock)completionBlock;

/**
 *  同步检查磁盘缓存中是否存在图像数据(不加载图像)
 *
 *  @param key             描述url的键。
 */
- (BOOL)diskImageDataExistsWithKey:(nullable NSString *)key;

/**
 * 操作以异步方式查询缓存，并在完成时调用完成。
 */
- (nullable NSOperation *)queryCacheOperationForKey:(nullable NSString *)key done:(nullable SDCacheQueryCompletedBlock)doneBlock;

/**
 * 操作以异步方式查询缓存，并在完成时调用完成。
 */
- (nullable NSOperation *)queryCacheOperationForKey:(nullable NSString *)key options:(SDImageCacheOptions)options done:(nullable SDCacheQueryCompletedBlock)doneBlock;

/**
 * 同步查询内存缓存。
 */
- (nullable UIImage *)imageFromMemoryCacheForKey:(nullable NSString *)key;

/**
 * 同时查询磁盘缓存。
 *
 * @param key The unique key used to store the image
 */
- (nullable UIImage *)imageFromDiskCacheForKey:(nullable NSString *)key;

/**
 * 在检查内存缓存后，同步查询缓存(内存和磁盘)。
 *
 * @param key The unique key used to store the image
 */
- (nullable UIImage *)imageFromCacheForKey:(nullable NSString *)key;

#pragma mark - Remove Ops

/**
 * 异步地从内存和磁盘缓存中删除映像。
 */
- (void)removeImageForKey:(nullable NSString *)key withCompletion:(nullable SDWebImageNoParamsBlock)completion;

/**
 * 从内存中删除映像，并异步地选择磁盘缓存。
 */
- (void)removeImageForKey:(nullable NSString *)key fromDisk:(BOOL)fromDisk withCompletion:(nullable SDWebImageNoParamsBlock)completion;

#pragma mark - Cache clean Ops

/**
 * 清除所有内存缓存图像。
 */
- (void)clearMemory;

/**
 * 异步清除所有磁盘缓存映像。非阻塞方法——立即返回。
 */
- (void)clearDiskOnCompletion:(nullable SDWebImageNoParamsBlock)completion;

/**
 * Async从磁盘中删除所有过期的缓存镜像。非阻塞方法——立即返回。
 */
- (void)deleteOldFilesWithCompletionBlock:(nullable SDWebImageNoParamsBlock)completionBlock;

#pragma mark - Cache Info

/**
 * 获取磁盘缓存使用的大小。
 */
- (NSUInteger)getSize;

/**
 * 获取磁盘缓存中的图像数量。
 */
- (NSUInteger)getDiskCount;

/**
 * 异步计算磁盘缓存的大小。
 */
- (void)calculateSizeWithCompletionBlock:(nullable SDWebImageCalculateSizeBlock)completionBlock;

#pragma mark - Cache Paths

/**
 *  获取某个键的缓存路径(需要缓存路径根文件夹)
 */
- (nullable NSString *)cachePathForKey:(nullable NSString *)key inPath:(nonnull NSString *)path;

/**
 *  获取某个键的默认高速缓存路径。
 */
- (nullable NSString *)defaultCachePathForKey:(nullable NSString *)key;

@end
