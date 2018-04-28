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

//这些方法用于支持取消UIView的图像加载，它被设计用于内部而不是外部。
//所有存储的操作都是弱的，所以在图像加载完成后将会被dalloced。如果您需要存储操作，请使用您自己的类来为它们保留一个强引用。
@interface UIView (WebCacheOperation)

/**
 *  设置图像加载操作(在基于UIView的弱映射表中存储)
 *
 *  @param operation the operation
 *  @param key       key for storing the operation
 */
- (void)sd_setImageLoadOperation:(nullable id<SDWebImageOperation>)operation forKey:(nullable NSString *)key;

/**
 *  取消当前UIView和key的所有操作。
 *
 *  @param key key for identifying the operations
 */
- (void)sd_cancelImageLoadOperationWithKey:(nullable NSString *)key;

/**
 *  只需删除与当前的UIView和key对应的操作，而不取消它们。
 *
 *  @param key key for identifying the operations
 */
- (void)sd_removeImageLoadOperationWithKey:(nullable NSString *)key;

@end

#endif
