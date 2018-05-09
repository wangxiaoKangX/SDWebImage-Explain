/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"
#import "NSData+ImageContentType.h"

/**
 一个布尔值，指示在解压缩过程中是否缩小大型图像。(NSNumber)
 */
FOUNDATION_EXPORT NSString * _Nonnull const SDWebImageCoderScaleDownLargeImagesKey;

/**
 返回使用CGColorSpaceCreateDeviceRGB创建的共享设备依赖的RGB颜色空间。

 @return The device-dependent RGB color space
 */
CG_EXTERN CGColorSpaceRef _Nonnull SDCGColorSpaceGetDeviceRGB(void);

/**
 检查CGImageRef是否包含alpha通道。

 @param imageRef The CGImageRef
 @return Return YES if CGImageRef contains alpha channel, otherwise return NO
 */
CG_EXTERN BOOL SDCGImageRefContainsAlpha(_Nullable CGImageRef imageRef);


/**
 这是图像编码器协议提供自定义图像解码/编码。
 这些方法都需要实现。
 @note Pay attention that these methods are not called from main queue.
 */
@protocol SDWebImageCoder <NSObject>

@required
#pragma mark - Decoding
/**
返回YES，如果这个编码器可以解码一些数据。否则，数据应该传递给另一个编码器。
 */
- (BOOL)canDecodeFromData:(nullable NSData *)data;

/**
将图像数据解码为图像。
 */
- (nullable UIImage *)decodedImageWithData:(nullable NSData *)data;

/**
用原始图像和图像数据解压图像。

 图像原始图像被解压。
 @数据指向原始图像数据的指针。指针本身是非空的，但是图像数据可以为空。如果需要，这些数据将被设置为缓存。如果您不需要在sametime中修改数据，请忽略此参数。
  optionsDict包含任何解压缩选项的字典。通过{SDWebImageCoderScaleDownLargeImagesKey: @(YES)}来缩小大型图像。
 @return解压图像
 */
- (nullable UIImage *)decompressedImageWithImage:(nullable UIImage *)image
                                            data:(NSData * _Nullable * _Nonnull)data
                                         options:(nullable NSDictionary<NSString*, NSObject*>*)optionsDict;

#pragma mark - Encoding

/**
 返回YES，如果这个编码者可以编码一些图像。否则，它应该被传递给另一个编码器。
 
格式的图像格式。
 如果编码器可以对图像进行编码，则不需要。
 */
- (BOOL)canEncodeToFormat:(SDImageFormat)format;

/**
 将图像编码为图像数据。

 图像被编码的图像。
 格式的图像格式编码，你应该注意' SDImageFormatUndefined '格式也是可能的。
 返回已编码的图像数据。
 */
- (nullable NSData *)encodedDataWithImage:(nullable UIImage *)image format:(SDImageFormat)format;

@end


/**
 这是图像编码协议，提供自定义的渐进式图像解码。
 这些方法都需要实现。
 注意，这些方法不是从主队列调用的。
 */
@protocol SDWebImageProgressiveCoder <SDWebImageCoder>

@required
/**
 返回YES，如果这个编码者可以增量解码一些数据。否则，它应该被传递给另一个编码器。
 
 数据的图像数据，以便我们可以查看它。
 如果这个编码器可以解码数据，则不然。
 */
- (BOOL)canIncrementallyDecodeFromData:(nullable NSData *)data;

/**
 增量解码图像数据到图像。
 
数据的图像数据已经下载到目前为止。
 完成下载是否完成。
 @警告，因为增量解码需要保持解码的上下文，我们将为每个下载操作分配一个新的实例，以避免冲突。
 从数据中返回解码的图像。
 */
- (nullable UIImage *)incrementallyDecodedImageWithData:(nullable NSData *)data finished:(BOOL)finished;

@end
