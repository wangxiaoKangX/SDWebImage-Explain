/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"
#import "SDWebImageOperation.h"
#import "SDWebImageDownloader.h"
#import "SDImageCache.h"

typedef NS_OPTIONS(NSUInteger, SDWebImageOptions) {
    /**
     * 默认情况下，当URL未被下载时，URL将被列入黑名单，这样库就不会继续尝试了。
     * 此标志禁用此黑名单。.
     */
    SDWebImageRetryFailed = 1 << 0,

    /**
     * 默认情况下，图像下载是在UI交互期间启动的，此标志禁用此功能，
     * 例如，导致UIScrollView减速的下载延迟。.
     */
    SDWebImageLowPriority = 1 << 1,

    /**
     *此标志在下载完成后禁用磁盘缓存，只在内存中缓存。
     */
    SDWebImageCacheMemoryOnly = 1 << 2,

    /**
     * 这个标志支持渐进式下载，在下载过程中，图像会以浏览器的方式逐步显示。
     * 默认情况下，图像只显示一次完全下载。
     */
    SDWebImageProgressiveDownload = 1 << 3,

    /**
     * 即使图像被缓存，也要尊重HTTP响应缓存控件，并在需要时从远程位置刷新图像。
     * 磁盘缓存将由NSURLCache处理，而不是SDWebImage，导致性能下降。
     *此选项帮助处理在相同请求URL后面的图像，例如Facebook graph api profile pics。
     *如果缓存的图像被刷新，完成块会被缓存后的图像调用一次，并再次调用最终的图像。
     *
     *仅当你不能使你的url静态的嵌入缓存破坏参数时才使用此标志。
     */
    SDWebImageRefreshCached = 1 << 4,

    /**
     *在ios4 +中，如果应用程序进入后台，继续下载图像。这是通过询问系统实现的。
     在后台额外的时间让请求完成。如果后台任务过期，操作将被取消。
     */
    SDWebImageContinueInBackground = 1 << 5,

    /**
     *通过设置来处理存储在NSHTTPCookieStore中的cookie。
     * NSMutableURLRequest。HTTPShouldHandleCookies =是的;
     */
    SDWebImageHandleCookies = 1 << 6,

    /**
     *启用不可信SSL证书。
     *用于测试目的。在生产中谨慎使用。
     */
    SDWebImageAllowInvalidSSLCertificates = 1 << 7,

    /***默认情况下，图像按排队顺序加载。这面旗帜使他们移动。
     排在队伍前面。
     */
    SDWebImageHighPriority = 1 << 8,
    
    /**
     *默认情况下，当图像加载时，占位符图像被加载。这面旗帜会延迟装货。
     *在图像完成加载后的占位符图像。
     */
    SDWebImageDelayPlaceholder = 1 << 9,

    /**
     *我们通常不会在动画图像上调用transformDownloadedImage委托方法，
     *大部分的转换代码都会损坏它。
     *使用这面旗帜来改变它们。
     */
    SDWebImageTransformAnimatedImage = 1 << 10,
    
    /**
     *默认情况下，下载后将图像添加到imageView。但在某些情况下，我们想。
     *在设置图像之前(例如，应用一个过滤器或将其添加到交叉渐变动画中)
     *如果您想在成功时手动设置图像，请使用此标志。
     */
    SDWebImageAvoidAutoSetImage = 1 << 11,
    
    /**
     *默认情况下，图像被解码，以尊重其原始大小。在iOS上，这面旗帜将会缩小。
     *图像大小与设备的受限内存兼容。
     *如果“SDWebImageProgressiveDownload”标志被设置，则向下的比例被停用。
     */
    SDWebImageScaleDownLargeImages = 1 << 12,
    
    /**
     *默认情况下，当图像缓存到内存中时，我们不会查询磁盘数据。此掩码可以同时强制查询磁盘数据。
     *此标志建议与“SDWebImageQueryDiskSync”一起使用，以确保在同一个runloop中加载图像。
     */
    SDWebImageQueryDataWhenInMemory = 1 << 13,
    
    /**
     *默认情况下，我们同步查询内存缓存，异步地查询磁盘缓存。这个掩码可以同步查询磁盘缓存，以确保在同一个runloop中加载图像。
     *如果您禁用内存缓存或在其他情况下，此标志可以避免在单元重用期间闪烁。
     */
    SDWebImageQueryDiskSync = 1 << 14,
    
    /***默认情况下，当缓存丢失时，图像将从网络下载。此标志可以防止网络仅从缓存加载。
     */
    SDWebImageFromCacheOnly = 1 << 15,
    /**
     *默认情况下，当您使用“SDWebImageTransition”来完成图像加载完成后的一些视图转换时，该转换仅适用于从网络下载的图像。这个掩码可以强制应用视图转换来实现内存和磁盘缓存。
     */
    SDWebImageForceTransition = 1 << 16
};

typedef void(^SDExternalCompletionBlock)(UIImage * _Nullable image, NSError * _Nullable error, SDImageCacheType cacheType, NSURL * _Nullable imageURL);

typedef void(^SDInternalCompletionBlock)(UIImage * _Nullable image, NSData * _Nullable data, NSError * _Nullable error, SDImageCacheType cacheType, BOOL finished, NSURL * _Nullable imageURL);

typedef NSString * _Nullable(^SDWebImageCacheKeyFilterBlock)(NSURL * _Nullable url);

typedef NSData * _Nullable(^SDWebImageCacheSerializerBlock)(UIImage * _Nonnull image, NSData * _Nullable data, NSURL * _Nullable imageURL);


@class SDWebImageManager;

@protocol SDWebImageManagerDelegate <NSObject>

@optional

/**
 * 当在缓存中未找到图像时，应该下载图像。
 *
 * @return 返回NO，以防止在缓存丢失时下载图像。如果没有实现，则是隐含的。
 */
- (BOOL)imageManager:(nonnull SDWebImageManager *)imageManager shouldDownloadImageForURL:(nullable NSURL *)imageURL;

/**
 *当下载错误发生时，控制复杂的逻辑以标记为失败的url。
 *如果委托实现了这个方法，我们将不会使用内置的方法来标记URL，因为它基于错误代码失败;
 */
- (BOOL)imageManager:(nonnull SDWebImageManager *)imageManager shouldBlockFailedURL:(nonnull NSURL *)imageURL withError:(nonnull NSError *)error;

/**
 *允许在下载后立即转换图像，然后在磁盘和内存中缓存它。
 *注意:这个方法是从全局队列中调用的，以避免阻塞主线程。
 */
- (nullable UIImage *)imageManager:(nonnull SDWebImageManager *)imageManager transformDownloadedImage:(nullable UIImage *)image withURL:(nullable NSURL *)imageURL;

@end

/**
 * SDWebImageManager是UIImageView+WebCache类别后面的类。
 *它将异步downloader (sdwebimagedloader)与图像缓存存储(SDImageCache)绑定在一起。
 *您可以使用这个类直接从web图像下载中受益，在另一个上下文中缓存。
 * UIView。
 *
 *这里有一个简单的例子，说明如何使用SDWebImageManager:
 *
 * @code

SDWebImageManager *manager = [SDWebImageManager sharedManager];
[manager loadImageWithURL:imageURL
                  options:0
                 progress:nil
                completed:^(UIImage *image, NSError *error, SDImageCacheType cacheType, BOOL finished, NSURL *imageURL) {
                    if (image) {
                        // do something with image
                    }
                }];

 * @endcode
 */
@interface SDWebImageManager : NSObject

@property (weak, nonatomic, nullable) id <SDWebImageManagerDelegate> delegate;

@property (strong, nonatomic, readonly, nullable) SDImageCache *imageCache;
@property (strong, nonatomic, readonly, nullable) SDWebImageDownloader *imageDownloader;

/**
 *高速缓存过滤器是每次SDWebImageManager需要将URL转换为缓存键时使用的块。这可以
 *用于删除图像URL的动态部分。
 *
*下面的示例在应用程序委托中设置一个过滤器，该过滤器将从该文件中删除任何查询字符串。
 * URL before to use it as a cache key:
 *
 * @code

SDWebImageManager.sharedManager.cacheKeyFilter = ^(NSURL * _Nullable url) {
    url = [[NSURL alloc] initWithScheme:url.scheme host:url.host path:url.path];
    return [url absoluteString];
};

 * @endcode
 */
@property (nonatomic, copy, nullable) SDWebImageCacheKeyFilterBlock cacheKeyFilter;

/**
 *缓存序列化器是一个块，用于将被解码的图像(源下载的数据)转换为用于存储到磁盘缓存的实际数据。如果返回nil，则意味着从映像实例生成数据，请参见“SDImageCache”。
 *例如，如果您正在使用WebP图像，并且在稍后从磁盘缓存中恢复时面对慢解码时的问题。您可以尝试将解码的图像编码到JPEG/PNG格式到磁盘缓存，而不是源下载的数据。
 * @注意“image”arg是非空的，但当您同时提供一个图像转换器并转换图像时，“数据”arg可能为nil，请注意这一情况。
 @note这个方法是从全局队列调用的，目的是不阻塞主线程。
 * @code
 SDWebImageManager.sharedManager.cacheSerializer = ^NSData * _Nullable(UIImage * _Nonnull image, NSData * _Nullable data, NSURL * _Nullable imageURL) {
    SDImageFormat format = [NSData sd_imageFormatForImageData:data];
    switch (format) {
        case SDImageFormatWebP:
            return image.images ? data : nil;
        default:
            return data;
    }
 };
 * @endcode
 * The default value is nil. Means we just store the source downloaded data to disk cache.
 */
@property (nonatomic, copy, nullable) SDWebImageCacheSerializerBlock cacheSerializer;

/**
 * 返回全球SDWebImageManager实例。
 *
 * @return SDWebImageManager shared instance
 */
+ (nonnull instancetype)sharedManager;

/**
 * 允许指定与图像管理器一起使用的缓存和图像下载器实例。
 使用指定的缓存和下载器返回“SDWebImageManager”的新实例。
 */
- (nonnull instancetype)initWithCache:(nonnull SDImageCache *)cache downloader:(nonnull SDWebImageDownloader *)downloader NS_DESIGNATED_INITIALIZER;

/**
 * 在给定的URL上下载图像，如果不存在缓存或返回缓存的版本。
 */
- (nullable id <SDWebImageOperation>)loadImageWithURL:(nullable NSURL *)url
                                              options:(SDWebImageOptions)options
                                             progress:(nullable SDWebImageDownloaderProgressBlock)progressBlock
                                            completed:(nullable SDInternalCompletionBlock)completedBlock;

/**
*为给定的URL保存图像缓存。
 *
 */

- (void)saveImageToCache:(nullable UIImage *)image forURL:(nullable NSURL *)url;

/**
 * 取消所有当前操作
 */
- (void)cancelAll;

/**
 * 检查一个或多个运行的操作。
 */
- (BOOL)isRunning;

/**
 * 异步检查图像是否已经被缓存。
 */
- (void)cachedImageExistsForURL:(nullable NSURL *)url
                     completion:(nullable SDWebImageCheckCacheCompletionBlock)completionBlock;

/**
 *  异步检查图像是否已经缓存到磁盘上。
 */
- (void)diskImageExistsForURL:(nullable NSURL *)url
                   completion:(nullable SDWebImageCheckCacheCompletionBlock)completionBlock;


/**
 *返回给定URL的缓存键。
 */
- (nullable NSString *)cacheKeyForURL:(nullable NSURL *)url;

@end
