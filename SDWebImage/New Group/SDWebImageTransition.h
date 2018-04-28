/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

#if SD_UIKIT || SD_MAC
#import "SDImageCache.h"

//这个类用于在视图类别载入图像完成后提供过渡动画。在UIView+WebCache.h中使用“sd_imageTransition”。
//对于UIKit(iOS & tvOS)，我们使用“+[UIView transitionWithView:duration:options:animation:completion]”用于过渡动画。
//对于AppKit(macOS)，我们使用“+[NSAnimationContext runAnimationGroup:completionHandler:]”用于过渡动画。您可以调用“+[NSAnimationContext currentContext]”来获取动画块中的上下文。
//这些过渡是为基本使用提供的。如果您需要复杂的动画，请考虑直接使用核心动画或使用“SDWebImageAvoidAutoSetImage”并在完成图像加载后实现您自己。

#if SD_UIKIT
typedef UIViewAnimationOptions SDWebImageAnimationOptions;
#else
typedef NS_OPTIONS(NSUInteger, SDWebImageAnimationOptions) {
    SDWebImageAnimationOptionAllowsImplicitAnimation = 1 << 0, // specify `allowsImplicitAnimation` for the `NSAnimationContext`
};
#endif

typedef void (^SDWebImageTransitionPreparesBlock)(__kindof UIView * _Nonnull view, UIImage * _Nullable image, NSData * _Nullable imageData, SDImageCacheType cacheType, NSURL * _Nullable imageURL);
typedef void (^SDWebImageTransitionAnimationsBlock)(__kindof UIView * _Nonnull view, UIImage * _Nullable image);
typedef void (^SDWebImageTransitionCompletionBlock)(BOOL finished);

@interface SDWebImageTransition : NSObject

/**
默认情况下，我们将图像设置为动画开始时的视图。您可以禁用它并提供自定义的设置映像过程。
 */
@property (nonatomic, assign) BOOL avoidAutoSetImage;
/**
 过渡动画的持续时间，以秒为单位。默认为0.5。
 */
@property (nonatomic, assign) NSTimeInterval duration;
/**
 在这个过渡动画(macOS)中所有动画使用的时间函数。
 */
@property (nonatomic, strong, nullable) CAMediaTimingFunction *timingFunction NS_AVAILABLE_MAC(10_7);
/**
 一个选项的蒙版，指示您如何执行动画。
 */
@property (nonatomic, assign) SDWebImageAnimationOptions animationOptions;
/**
 在动画序列开始之前要执行的块对象。
 */
@property (nonatomic, copy, nullable) SDWebImageTransitionPreparesBlock prepares;
/**
 一个块对象，它包含您想要对指定视图作出的更改。
 */
@property (nonatomic, copy, nullable) SDWebImageTransitionAnimationsBlock animations;
/**
 当动画序列结束时要执行的块对象。
 */
@property (nonatomic, copy, nullable) SDWebImageTransitionCompletionBlock completion;

@end

//方便的方式来创造过渡。如果需要，记得指定持续时间。
//对于UIKit，这些转换只使用对应的“animationOptions”
//对于AppKit，这些过渡使用“动画”中的核心动画。所以你的观点必须是有层次的。在应用之前设置“wantsLayer = YES”。

@interface SDWebImageTransition (Conveniences)

// class property is available in Xcode 8. We will drop the Xcode 7.3 support in 5.x
#if __has_feature(objc_class_property)
/// Fade transition.
@property (nonatomic, class, nonnull, readonly) SDWebImageTransition *fadeTransition;
/// Flip from left transition.
@property (nonatomic, class, nonnull, readonly) SDWebImageTransition *flipFromLeftTransition;
/// Flip from right transition.
@property (nonatomic, class, nonnull, readonly) SDWebImageTransition *flipFromRightTransition;
/// Flip from top transition.
@property (nonatomic, class, nonnull, readonly) SDWebImageTransition *flipFromTopTransition;
/// Flip from bottom transition.
@property (nonatomic, class, nonnull, readonly) SDWebImageTransition *flipFromBottomTransition;
/// Curl up transition.
@property (nonatomic, class, nonnull, readonly) SDWebImageTransition *curlUpTransition;
/// Curl down transition.
@property (nonatomic, class, nonnull, readonly) SDWebImageTransition *curlDownTransition;
#else
+ (nonnull instancetype)fadeTransition;
+ (nonnull instancetype)flipFromLeftTransition;
+ (nonnull instancetype)flipFromRightTransition;
+ (nonnull instancetype)flipFromTopTransition;
+ (nonnull instancetype)flipFromBottomTransition;
+ (nonnull instancetype)curlUpTransition;
+ (nonnull instancetype)curlDownTransition;
#endif

@end

#endif
