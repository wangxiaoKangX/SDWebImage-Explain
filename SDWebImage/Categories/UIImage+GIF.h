/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 * (c) Laurin Brandner
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

@interface UIImage (GIF)

/**
 *从一个NSData创建一个动画的UIImage。
 *对于静态GIF，将创建一个带有“images”数组的UIImage设置为nil。对于GIF动画，将创建一个具有有效的“图像”数组的UIImage。
 */
+ (UIImage *)sd_animatedGIFWithData:(NSData *)data;

/**
 *  检查UIImage实例是否是GIF。将使用“图像”数组。
 */
- (BOOL)isGIF;

@end
