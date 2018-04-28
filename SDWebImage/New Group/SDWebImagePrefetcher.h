/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageManager.h"

@class SDWebImagePrefetcher;

@protocol SDWebImagePrefetcherDelegate <NSObject>

@optional

/**
 * 当图像被预取时调用。
 *
 * @param imagePrefetcher 当前图像prefetcher
 * @param imageURL        预取的图像url。
 * @param finishedCount   预取的图像总数(成功与否)
 * @param totalCount      要预取的图像的总数。
 */
- (void)imagePrefetcher:(nonnull SDWebImagePrefetcher *)imagePrefetcher didPrefetchURL:(nullable NSURL *)imageURL finishedCount:(NSUInteger)finishedCount totalCount:(NSUInteger)totalCount;

/**
 * 当所有图像都是预取时调用。
 */
- (void)imagePrefetcher:(nonnull SDWebImagePrefetcher *)imagePrefetcher didFinishWithTotalCount:(NSUInteger)totalCount skippedCount:(NSUInteger)skippedCount;

@end

typedef void(^SDWebImagePrefetcherProgressBlock)(NSUInteger noOfFinishedUrls, NSUInteger noOfTotalUrls);
typedef void(^SDWebImagePrefetcherCompletionBlock)(NSUInteger noOfFinishedUrls, NSUInteger noOfSkippedUrls);

/**
 * 在缓存中预取一些url以供将来使用。图片在低优先级下载。
 */
@interface SDWebImagePrefetcher : NSObject

/**
 *  网络图片管理器
 */
@property (strong, nonatomic, readonly, nonnull) SDWebImageManager *manager;

/**
 * 同时预取的url的最大数量。默认为3。
 */
@property (nonatomic, assign) NSUInteger maxConcurrentDownloads;

/**
 * SDWebImageOptions prefetcher。默认为SDWebImageLowPriority。
 */
@property (nonatomic, assign) SDWebImageOptions options;

/**
 * 队列Prefetcher选项。默认为主要的队列。
 */
@property (strong, nonatomic, nonnull) dispatch_queue_t prefetcherQueue;

@property (weak, nonatomic, nullable) id <SDWebImagePrefetcherDelegate> delegate;

/**
 * 返回全局图像预取器实例。
 */
+ (nonnull instancetype)sharedImagePrefetcher;

/**
 * 允许您使用任意图像管理器实例化一个预取器。
 */
- (nonnull instancetype)initWithImageManager:(nonnull SDWebImageManager *)manager NS_DESIGNATED_INITIALIZER;

/**
 *指定url的列表，让SDWebImagePrefetcher来排队预取，
 *现时只下载一幅图像，
 *并跳过下载失败的图片，然后进入列表中的下一个图像。
 *任何预先运行的预取操作都被取消。
 *
 * @param urls list of URLs to prefetch  v. 预取
 */
- (void)prefetchURLs:(nullable NSArray<NSURL *> *)urls;

/**
 *指定url的列表，让SDWebImagePrefetcher来排队预取，
 *现时只下载一幅图像，
 *并跳过下载失败的图片，然后进入列表中的下一个图像。
 *任何预先运行的预取操作都被取消。
 */
- (void)prefetchURLs:(nullable NSArray<NSURL *> *)urls
            progress:(nullable SDWebImagePrefetcherProgressBlock)progressBlock
           completed:(nullable SDWebImagePrefetcherCompletionBlock)completionBlock;

/**
 * 删除和取消排队列表。
 */
- (void)cancelPrefetching;


@end
