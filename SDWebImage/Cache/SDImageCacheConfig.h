/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"

@interface SDImageCacheConfig : NSObject

/**
 * 下载和缓存的压缩映像可以提高性能，但可以消耗大量内存。
 * 默认值为YES。如果由于内存消耗过大而导致崩溃，请将此设置为NO。
 */
@property (assign, nonatomic) BOOL shouldDecompressImages;

/**
 * 禁用iCloud备份[默认为YES]
 */
@property (assign, nonatomic) BOOL shouldDisableiCloud;

/**
 * 使用内存缓存[默认为YES]
 */
@property (assign, nonatomic) BOOL shouldCacheImagesInMemory;

/**
 * 读取磁盘缓存时的阅读选项。
 * 默认值为0。您可以将其设置为“NSDataReadingMappedIfSafe”以提高性能。
 */
@property (assign, nonatomic) NSDataReadingOptions diskCacheReadingOptions;

/**
 * 在将缓存写入磁盘时写入选项。
 * 默认为“NSDataWritingAtomic”。您可以将其设置为“NSDataWritingWithoutOverwriting”以防止覆盖现有文件。
 */
@property (assign, nonatomic) NSDataWritingOptions diskCacheWritingOptions;

/**
 * 在缓存中保存图像的最长时间，以秒为单位。
 */
@property (assign, nonatomic) NSInteger maxCacheAge;

/**
 * 缓存的最大大小，以字节为单位。
 */
@property (assign, nonatomic) NSUInteger maxCacheSize;

@end
