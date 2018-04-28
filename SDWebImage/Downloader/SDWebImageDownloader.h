/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"
#import "SDWebImageOperation.h"

typedef NS_OPTIONS(NSUInteger, SDWebImageDownloaderOptions) {
    /**
     * 将下载放在低队列优先级和任务优先级。
     */
    SDWebImageDownloaderLowPriority = 1 << 0,
    
    /**
     * 这个标志支持渐进式下载，在下载过程中，图像会以浏览器的方式逐步显示。
     */
    SDWebImageDownloaderProgressiveDownload = 1 << 1,

    /**
     * 默认情况下，请求阻止使用NSURLCache。这个标志,NSURLCache
     *用于默认策略。
     */
    SDWebImageDownloaderUseNSURLCache = 1 << 2,

    /**
     * 如果从NSURLCache读取图像，则使用nil图像/imageData调用完成块。
     *(与“sdwebimagedloaderusensurlcache”相结合)。
     */
    SDWebImageDownloaderIgnoreCachedResponse = 1 << 3,
    
    /**
     * 在iOS 4+中，如果应用程序进入后台，继续下载图像。这是通过询问系统实现的。
     在后台额外的时间让请求完成。如果后台任务过期，操作将被取消。
     */
    SDWebImageDownloaderContinueInBackground = 1 << 4,

    /**
     *通过设置来处理存储在NSHTTPCookieStore中的cookie。
     * NSMutableURLRequest。HTTPShouldHandleCookies =是的;
     */
    SDWebImageDownloaderHandleCookies = 1 << 5,

    /**
     *启用不可信SSL证书。
     *用于测试目的。在生产中谨慎使用。
     */
    SDWebImageDownloaderAllowInvalidSSLCertificates = 1 << 6,

    /**
     * 将下载放在高队列优先级和任务优先级。
     */
    SDWebImageDownloaderHighPriority = 1 << 7,
    
    /**
     * 缩小图片
     */
    SDWebImageDownloaderScaleDownLargeImages = 1 << 8,
};

typedef NS_ENUM(NSInteger, SDWebImageDownloaderExecutionOrder) {
    /**
     * 默认值。所有下载操作将以队列样式执行(先入先出)。
     */
    SDWebImageDownloaderFIFOExecutionOrder,

    /**
     * 所有的下载操作都将以堆栈的方式执行(最后先出)。
     */
    SDWebImageDownloaderLIFOExecutionOrder
};

FOUNDATION_EXPORT NSString * _Nonnull const SDWebImageDownloadStartNotification;
FOUNDATION_EXPORT NSString * _Nonnull const SDWebImageDownloadStopNotification;

typedef void(^SDWebImageDownloaderProgressBlock)(NSInteger receivedSize, NSInteger expectedSize, NSURL * _Nullable targetURL);

typedef void(^SDWebImageDownloaderCompletedBlock)(UIImage * _Nullable image, NSData * _Nullable data, NSError * _Nullable error, BOOL finished);

typedef NSDictionary<NSString *, NSString *> SDHTTPHeadersDictionary;
typedef NSMutableDictionary<NSString *, NSString *> SDHTTPHeadersMutableDictionary;

typedef SDHTTPHeadersDictionary * _Nullable (^SDWebImageDownloaderHeadersFilterBlock)(NSURL * _Nullable url, SDHTTPHeadersDictionary * _Nullable headers);

/**
 *  与每个下载相关联的标记。可以用来取消下载吗?
 */
@interface SDWebImageDownloadToken : NSObject <SDWebImageOperation>

/**
下载的URL。这应该是只读的，您不应该修改。
 */
@property (nonatomic, strong, nullable) NSURL *url;
/**
 从“addHandlersForProgress:completed”中删除的标记。这应该是只读的，您不应该修改。
 @note use `-[SDWebImageDownloadToken cancel]` to cancel the token
 */
@property (nonatomic, strong, nullable) id downloadOperationCancelToken;

@end


/**
 * 异步下载器专用和优化的图像加载。
 */
@interface SDWebImageDownloader : NSObject

/**
 * 下载和缓存的压缩映像可以提高性能，但可以消耗大量内存。
 默认值为YES。如果由于内存消耗过大而导致崩溃，请将此设置为NO。
 */
@property (assign, nonatomic) BOOL shouldDecompressImages;

/**
 * 并发下载的最大数量。
 */
@property (assign, nonatomic) NSInteger maxConcurrentDownloads;

/**
 * 显示仍然需要下载的当前下载数量。
 */
@property (readonly, nonatomic) NSUInteger currentDownloadCount;

/**
 *  下载操作的超时值(以秒为单位)。默认值:15.0。
 */
@property (assign, nonatomic) NSTimeInterval downloadTimeout;

/**
 * 内部NSURLSession使用的配置。
 *直接改变这个物体没有效果。
 */
@property (readonly, nonatomic, nonnull) NSURLSessionConfiguration *sessionConfiguration;


/**
 * 更改下载操作执行顺序。默认值是“SDWebImageDownloaderFIFOExecutionOrder”。
 */
@property (assign, nonatomic) SDWebImageDownloaderExecutionOrder executionOrder;

/**
 *  单例方法，返回共享实例。
 */
+ (nonnull instancetype)sharedDownloader;

/**
 * 设置要为请求操作设置的默认URL凭据。
 */
@property (strong, nonatomic, nullable) NSURLCredential *urlCredential;

/**
 * Set username
 */
@property (strong, nonatomic, nullable) NSString *username;

/**
 * Set password
 */
@property (strong, nonatomic, nullable) NSString *password;

/**
 * 设置筛选器来选择下载图像HTTP请求的头。
 *
 * 将为每个下载图像请求调用此块，返回。
 * NSDictionary将在相应的HTTP请求中作为header使用。
 */
@property (nonatomic, copy, nullable) SDWebImageDownloaderHeadersFilterBlock headersFilter;

/**
创建具有指定会话配置的downloader实例。
 */
- (nonnull instancetype)initWithSessionConfiguration:(nullable NSURLSessionConfiguration *)sessionConfiguration NS_DESIGNATED_INITIALIZER;

/**
 * 为每个下载HTTP请求附加一个HTTP头的值
 */
- (void)setValue:(nullable NSString *)value forHTTPHeaderField:(nullable NSString *)field;

/**
 * 返回指定的HTTP头字段的值。
 *
 * @返回与header字段字段关联的值，如果没有相应的头字段，则返回“nil”。
 */
- (nullable NSString *)valueForHTTPHeaderField:(nullable NSString *)field;

/**
 * 将“SDWebImageDownloaderOperation”的子类设置为默认值。
 *“NSOperation”每一次使用SDWebImage构造一个请求。
 *下载图像的操作。
 *
 * @param operationClass:“SDWebImageDownloaderOperation”的子类来设置。
 *为默认。传递“nil”将恢复到“sdwebimageownloaderoperation”。
 */
- (void)setOperationClass:(nullable Class)operationClass;

/**
 * 创建一个带有给定URL的SDWebImageDownloader实例。
 *
*当图像完成下载或发生错误时，将通知该委托。 *
 * @see SDWebImageDownloaderDelegate
 */
- (nullable SDWebImageDownloadToken *)downloadImageWithURL:(nullable NSURL *)url
                                                   options:(SDWebImageDownloaderOptions)options
                                                  progress:(nullable SDWebImageDownloaderProgressBlock)progressBlock
                                                 completed:(nullable SDWebImageDownloaderCompletedBlock)completedBlock;

/**
 * 取消先前排队使用-downloadImageWithURL的下载:选项:进度:完成:
 */
- (void)cancel:(nullable SDWebImageDownloadToken *)token;

/**
 * 设置下载队列暂停状态。
 */
- (void)setSuspended:(BOOL)suspended;

/**
 * 取消队列中的所有下载操作。
 */
- (void)cancelAllDownloads;

/**
 * 强制SDWebImageDownloader创建和使用一个新的NSURLSession。
 用给定的配置初始化。
 */
- (void)createNewSessionWithConfiguration:(nonnull NSURLSessionConfiguration *)sessionConfiguration;

/**
 * 使托管会话失效，可选择性地取消挂起的操作。
 * @注意，如果您使用自定义downloader而不是共享的downloader，那么当您不使用它来避免内存泄漏时，您需要调用这个方法。
 * @param 取消操作是否取消挂起的操作。.
 * @注意，在共享下载器上调用此方法没有效果。
 */
- (void)invalidateSessionAndCancel:(BOOL)cancelPendingOperations;

@end
