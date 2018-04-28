/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

#if SD_UIKIT || SD_MAC

#import "SDWebImageManager.h"
#import "SDWebImageTransition.h"

/**
 一个调度组来维护setImageBlock和completionBlock。这个键只能在内部使用，将来可能会改变。(dispatch_group_t)
 */
FOUNDATION_EXPORT NSString * _Nonnull const SDWebImageInternalSetImageGroupKey;
/**
 一个SDWebImageManager实例，用于控制在UIImageView+WebCache类别中使用的图像下载和缓存过程。如果没有提供，请使用共享管理器(SDWebImageManager)
 */
FOUNDATION_EXPORT NSString * _Nonnull const SDWebImageExternalCustomManagerKey;
/**
 该值指定不能确定图像进度单元数，因为没有调用progressBlock。
 */
FOUNDATION_EXPORT const int64_t SDWebImageProgressUnitCountUnknown; /* 1LL */

typedef void(^SDSetImageBlock)(UIImage * _Nullable image, NSData * _Nullable imageData);

@interface UIView (WebCache)

/**
 * 获取当前图像URL。
 *
 * @请注意，由于类别的限制，如果您使用setImage，则该属性可能会失去同步:直接。
 */
- (nullable NSURL *)sd_imageURL;

/**
 * 与视图关联的当前图像加载进度。单位计数是收到的大小和例外的大小的下载。
 * “totalUnitCount”和“completedUnitCount”将在一个新的图像加载启动后(从当前队列更改)重置为0。他们将被设置为“SDWebImageProgressUnitCountUnknown”，如果没有调用progressBlock，但是图像加载成功标志着进程的完成(从主队列更改)。
 * @注意，您可以在进度上使用键值观察，但是您应该注意到，在下载过程中，从后台队列中获得的更改(与progressBlock相同)。如果您想要使用KVO并更新UI，请确保在主队列上调度。它是推荐的，使用一些KVO libs，比如KVOController，因为它更安全，更容易使用。.
 * @注意，如果值为nil, getter将创建一个进度实例。您还可以设置一个自定义进度实例，并在图像加载期间更新它。
 * @注意，由于类别的限制，如果您直接更新进度，此属性可能会失去同步。
 */
@property (nonatomic, strong, null_resettable) NSProgress *sd_imageProgress;

/**
 * 将imageView ' image '设置为' url '和可选的占位符图像
 */
- (void)sd_internalSetImageWithURL:(nullable NSURL *)url
                  placeholderImage:(nullable UIImage *)placeholder
                           options:(SDWebImageOptions)options
                      operationKey:(nullable NSString *)operationKey
                     setImageBlock:(nullable SDSetImageBlock)setImageBlock
                          progress:(nullable SDWebImageDownloaderProgressBlock)progressBlock
                         completed:(nullable SDExternalCompletionBlock)completedBlock;

/**
 * 将imageView ' image '设置为' url '和可选的占位符图像。
 */
- (void)sd_internalSetImageWithURL:(nullable NSURL *)url
                  placeholderImage:(nullable UIImage *)placeholder
                           options:(SDWebImageOptions)options
                      operationKey:(nullable NSString *)operationKey
                     setImageBlock:(nullable SDSetImageBlock)setImageBlock
                          progress:(nullable SDWebImageDownloaderProgressBlock)progressBlock
                         completed:(nullable SDExternalCompletionBlock)completedBlock
                           context:(nullable NSDictionary<NSString *, id> *)context;

/**
 * 取消当前的图像负载。
 */
- (void)sd_cancelCurrentImageLoad;

#pragma mark - Image Transition

/**
 图像加载完成后的图像转换。看到“SDWebImageTransition”。
 如果指定nil，则不要进行转换。Defautls为零。
 */
@property (nonatomic, strong, nullable) SDWebImageTransition *sd_imageTransition;

#if SD_UIKIT

#pragma mark - Activity indicator

/**
 *  展示活动UIActivityIndicatorView
 */
- (void)sd_setShowActivityIndicatorView:(BOOL)show;

/**
 *  设置所需的UIActivityIndicatorViewStyle
 */
- (void)sd_setIndicatorStyle:(UIActivityIndicatorViewStyle)style;

- (BOOL)sd_showActivityIndicatorView;
- (void)sd_addActivityIndicator;
- (void)sd_removeActivityIndicator;

#endif

@end

#endif
