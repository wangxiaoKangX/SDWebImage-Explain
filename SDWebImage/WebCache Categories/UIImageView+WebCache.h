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



@interface UIImageView (WebCache)

/**
 * 设置imageView ' image '和' url '。
 *
 * 下载是异步的和缓存的。
 *
 * @param url为图像的url。
 */
- (void)sd_setImageWithURL:(nullable NSURL *)url NS_REFINED_FOR_SWIFT;

/**
 * 将imageView ' image '设置为' url '和占位符。
 *
 * 下载是异步的和缓存的。
 */
- (void)sd_setImageWithURL:(nullable NSURL *)url
          placeholderImage:(nullable UIImage *)placeholder NS_REFINED_FOR_SWIFT;

/**
 * 设置imageView ' image '和' url '，占位符和自定义选项。
 *
 * 下载是异步的和缓存的。
 */
- (void)sd_setImageWithURL:(nullable NSURL *)url
          placeholderImage:(nullable UIImage *)placeholder
                   options:(SDWebImageOptions)options NS_REFINED_FOR_SWIFT;

/**
 * 设置imageView ' image '和' url '。
 *
 * 下载是异步的和缓存的。
 */
- (void)sd_setImageWithURL:(nullable NSURL *)url
                 completed:(nullable SDExternalCompletionBlock)completedBlock;

/**
 * 设置imageView ' image '与' url '，占位符。
 *
 * 下载是异步的和缓存的。
 */
- (void)sd_setImageWithURL:(nullable NSURL *)url
          placeholderImage:(nullable UIImage *)placeholder
                 completed:(nullable SDExternalCompletionBlock)completedBlock NS_REFINED_FOR_SWIFT;

/**
 * 设置imageView ' image '和' url '，占位符和自定义选项。.
 *
 * 下载是异步的和缓存的
 */
- (void)sd_setImageWithURL:(nullable NSURL *)url
          placeholderImage:(nullable UIImage *)placeholder
                   options:(SDWebImageOptions)options
                 completed:(nullable SDExternalCompletionBlock)completedBlock;

/**
 * 设置imageView ' image '和' url '，占位符和自定义选项。
 */
- (void)sd_setImageWithURL:(nullable NSURL *)url
          placeholderImage:(nullable UIImage *)placeholder
                   options:(SDWebImageOptions)options
                  progress:(nullable SDWebImageDownloaderProgressBlock)progressBlock
                 completed:(nullable SDExternalCompletionBlock)completedBlock;

/**
 * 设置imageView ' image '和' url '和自定义选项。占位符映像来自先前的缓存映像，如果查询失败，将使用提供的映像。
 * 这种方法的设计是为了确保在同一个runloop中发生了占位符和查询缓存过程，以避免在两个“setImage:调用”期间在单元格上闪烁。但这确实是一种误解和弃用。
 * 这可以通过使用“sd_setImageWithURL:”和“SDWebImageQueryDiskSync”来实现。但是要注意的是，如果内存缓存丢失，则同步查询磁盘缓存可能会降低帧速率。
 */
- (void)sd_setImageWithPreviousCachedImageWithURL:(nullable NSURL *)url
                                 placeholderImage:(nullable UIImage *)placeholder
                                          options:(SDWebImageOptions)options
                                         progress:(nullable SDWebImageDownloaderProgressBlock)progressBlock
                                        completed:(nullable SDExternalCompletionBlock)completedBlock __deprecated_msg("This method is misunderstanding and deprecated, consider using `SDWebImageQueryDiskSync` options with `sd_setImageWithURL:` instead");

#if SD_UIKIT

#pragma mark - Animation of multiple images

/**
 * 下载一组图像并在一个动画循环中启动它们。
 *
 * @param 数组NSURL。
 */
- (void)sd_setAnimationImagesWithURLs:(nonnull NSArray<NSURL *> *)arrayOfURLs;

- (void)sd_cancelCurrentAnimationImagesLoad;

#endif

@end

#endif
