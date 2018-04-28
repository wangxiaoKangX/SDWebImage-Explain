//
//  ViewController.m
//  SDlianxi
//
//  Created by 王晓康 on 2018/4/28.
//  Copyright © 2018年 wxk. All rights reserved.
//

#import "ViewController.h"
#import "UIImageView+WebCache.h"
#import "UIView+WebCache.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    UIImageView * imageV = [[UIImageView alloc] init];
    
    [imageV sd_setImageWithURL:nil placeholderImage:nil];
    
    NSDictionary * dic;
    
    NSMapTable * mapT = [NSMapTable mapTableWithKeyOptions:NSMapTableCopyIn valueOptions:NSMapTableStrongMemory];
    
    UIView * view = [[UIView alloc] init];
//    [view sdintter]
    
}


@end
