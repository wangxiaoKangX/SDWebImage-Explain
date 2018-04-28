/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifdef SD_WEBP

#import "SDWebImageCompat.h"

@interface UIImage (WebP)

/**
 *获取当前WebP图像循环计数，默认值为0。
 *对于静态WebP图像，值为0。
 *对于动画WebP图像，0表示无限重复动画。
 *注意，由于类别的限制，这个属性可能会失去同步。
 *如果您使用CGImage或其他方法创建另一个实例。
 * @return WebP图像循环计数。
 使用“sd_imageLoopCount”代替。
 */
- (NSInteger)sd_webpLoopCount __deprecated_msg("Method deprecated. Use `sd_imageLoopCount` in `UIImage+MultiFormat.h`");

+ (nullable UIImage *)sd_imageWithWebPData:(nullable NSData *)data;

@end

#endif
