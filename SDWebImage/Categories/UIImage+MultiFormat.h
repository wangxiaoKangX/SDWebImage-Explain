/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"
#import "NSData+ImageContentType.h"

@interface UIImage (MultiFormat)

/**
 * UIKit:
 *对于静态图像格式，这个值总是0。
 *对于动画图像格式，0意味着无限循环。
 *注意，由于类别的限制，如果您使用CGImage或其他方法创建另一个实例，则该属性可能会失去同步。
 * AppKit:
 * NSImage目前只支持GIF imageRep动画，不像UIImage。
 *此属性的getter将从GIF imageRep获得循环计数。
 *此属性的setter将从GIF imageRep设置循环计数。
 */
@property (nonatomic, assign) NSUInteger sd_imageLoopCount;

+ (nullable UIImage *)sd_imageWithData:(nullable NSData *)data;
- (nullable NSData *)sd_imageData;
- (nullable NSData *)sd_imageDataAsFormat:(SDImageFormat)imageFormat;

@end
