//
//  PPResultViewController.h
//  pdf417-core-demo
//
//  Created by Jura on 06/12/13.
//  Copyright (c) 2013 PhotoPay. All rights reserved.
//

#import <UIKit/UIKit.h>

@class PPScanResult;

@interface PPResultViewController : UIViewController

@property (strong, nonatomic) PPScanResult* scanResult;

@property (weak, nonatomic) IBOutlet UIScrollView *scrollView;

@property (weak, nonatomic) IBOutlet UILabel *textResultHeader;

@property (weak, nonatomic) IBOutlet UIView *textResultDelimiter;

@property (weak, nonatomic) IBOutlet UILabel *textResultLabel;

@property (weak, nonatomic) IBOutlet UILabel *bytesResultHeader;

@property (weak, nonatomic) IBOutlet UIView *bytesResultDelimiter;

@property (weak, nonatomic) IBOutlet UILabel *bytesResultLabel;

@end
