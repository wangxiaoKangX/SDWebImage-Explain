/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageDownloader.h"
#import "SDWebImageOperation.h"

FOUNDATION_EXPORT NSString * _Nonnull const SDWebImageDownloadStartNotification;
FOUNDATION_EXPORT NSString * _Nonnull const SDWebImageDownloadReceiveResponseNotification;
FOUNDATION_EXPORT NSString * _Nonnull const SDWebImageDownloadStopNotification;
FOUNDATION_EXPORT NSString * _Nonnull const SDWebImageDownloadFinishNotification;



/**
 描述了一个下载操作。如果想要使用自定义的downloader op，则需要从“NSOperation”继承并遵循该协议。
 有关这些方法的描述，请参见“SDWebImageDownloaderOperation”
 */
@protocol SDWebImageDownloaderOperationInterface<NSObject>

- (nonnull instancetype)initWithRequest:(nullable NSURLRequest *)request
                              inSession:(nullable NSURLSession *)session
                                options:(SDWebImageDownloaderOptions)options;

- (nullable id)addHandlersForProgress:(nullable SDWebImageDownloaderProgressBlock)progressBlock
                            completed:(nullable SDWebImageDownloaderCompletedBlock)completedBlock;

- (BOOL)shouldDecompressImages;
- (void)setShouldDecompressImages:(BOOL)value;

- (nullable NSURLCredential *)credential;
- (void)setCredential:(nullable NSURLCredential *)value;

- (BOOL)cancel:(nullable id)token;

@end


@interface SDWebImageDownloaderOperation : NSOperation <SDWebImageDownloaderOperationInterface, SDWebImageOperation, NSURLSessionTaskDelegate, NSURLSessionDataDelegate>

/**
 * 操作任务所使用的请求。
 */
@property (strong, nonatomic, readonly, nullable) NSURLRequest *request;

/**
 * 操作的任务
 */
@property (strong, nonatomic, readonly, nullable) NSURLSessionTask *dataTask;


@property (assign, nonatomic) BOOL shouldDecompressImages;

/**
 *  用于确定URL连接是否应该查询凭证存储以验证连接。
 *  @不赞成使用几个版本。
 */
@property (nonatomic, assign) BOOL shouldUseCredentialStorage __deprecated_msg("Property deprecated. Does nothing. Kept only for backwards compatibility");

/**
 * 在“-URLSession:任务:didReceiveChallenge:completionHandler:”中用于身份验证挑战的凭据。
 *
 * 如果存在请求URL的用户名或密码的任何共享凭证，则会覆盖此功能。
 */
@property (nonatomic, strong, nullable) NSURLCredential *credential;

/**
 * 用于接收方的sdwebimageownloaderoptions。
 */
@property (assign, nonatomic, readonly) SDWebImageDownloaderOptions options;

/**
 * 数据的预期大小。
 */
@property (assign, nonatomic) NSInteger expectedSize;

/**
 * 操作返回的响应。
 */
@property (strong, nonatomic, nullable) NSURLResponse *response;

/**
 *  初始化一个“SDWebImageDownloaderOperation”对象
 *
 *  @see SDWebImageDownloaderOperation
 *
 *  @param request        the URL request
 *  @param session        该操作将运行的URL会话。
 *  @param options        downloader options
 *
 *  @return the initialized instance
 */
- (nonnull instancetype)initWithRequest:(nullable NSURLRequest *)request
                              inSession:(nullable NSURLSession *)session
                                options:(SDWebImageDownloaderOptions)options NS_DESIGNATED_INITIALIZER;

/**
 *  为进度和完成添加处理程序。返回一个tokent，它可以被传递到-cancel:取消这个集合。
 *回调。
 */
- (nullable id)addHandlersForProgress:(nullable SDWebImageDownloaderProgressBlock)progressBlock
                            completed:(nullable SDWebImageDownloaderCompletedBlock)completedBlock;

/**
取消一组回调。一旦所有回调被取消，操作将被取消。 
 */
- (BOOL)cancel:(nullable id)token;

@end
