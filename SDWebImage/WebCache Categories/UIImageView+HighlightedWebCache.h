/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

#if SD_UIKIT

#import "SDWebImageManager.h"

/**
 * 集成了SDWebImage异步下载和缓存的远程图像与UIImageView的高亮显示状态。
 */
@interface UIImageView (HighlightedWebCache)

/**
 * 设置imageView ' highlightedImage '和' url '。
 */
- (void)sd_setHighlightedImageWithURL:(nullable NSURL *)url NS_REFINED_FOR_SWIFT;

/**
 * 设置imageView ' highlightedImage '和' url '和自定义选项。
 */
- (void)sd_setHighlightedImageWithURL:(nullable NSURL *)url
                              options:(SDWebImageOptions)options NS_REFINED_FOR_SWIFT;

/**
 * 使用“url”设置imageView ' highlightedImage '.
 */
- (void)sd_setHighlightedImageWithURL:(nullable NSURL *)url
                            completed:(nullable SDExternalCompletionBlock)completedBlock NS_REFINED_FOR_SWIFT;

/**
 * 设置imageView ' highlightedImage '和' url '和自定义选项。.
 */
- (void)sd_setHighlightedImageWithURL:(nullable NSURL *)url
                              options:(SDWebImageOptions)options
                            completed:(nullable SDExternalCompletionBlock)completedBlock;

/**
 * 设置imageView ' highlightedImage '和' url '和自定义选项。 
 */
- (void)sd_setHighlightedImageWithURL:(nullable NSURL *)url
                              options:(SDWebImageOptions)options
                             progress:(nullable SDWebImageDownloaderProgressBlock)progressBlock
                            completed:(nullable SDExternalCompletionBlock)completedBlock;

@end

#endif
