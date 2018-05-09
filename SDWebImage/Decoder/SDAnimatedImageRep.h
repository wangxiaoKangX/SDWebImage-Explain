/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

#if SD_MAC

// “NSBitmapImageRep”的一个子类来修复这个GIF循环数问题，因为“NSBitmapImageRep”将使用“kCGImagePropertyGIFDelayTime”来重置“NSImageCurrentFrameDuration”，而不是“kCGImagePropertyGIFUnclampedDelayTime”。
// 构建在GIF编码中使用这个代替“NSBitmapImageRep”来更好的GIF渲染。如果你不想要这个，只需要启用“SDWebImageImageIOCoder”，它只调用“NSImage”API，并实际使用“NSBitmapImageRep”作为GIF图像。

@interface SDAnimatedImageRep : NSBitmapImageRep

@end

#endif
