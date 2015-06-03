//
//  PPResultViewController.m
//  pdf417-core-demo
//
//  Created by Jura on 06/12/13.
//  Copyright (c) 2013 PhotoPay. All rights reserved.
//

#import "PPResultViewController.h"
#import "PPScanResult.h"

@interface PPResultViewController ()

@end

@implementation PPResultViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    if ([self respondsToSelector:@selector(edgesForExtendedLayout)]) {
        self.edgesForExtendedLayout = UIRectEdgeNone;
    }
    
    self.title = @"Result";
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)adjustBytesLabels:(CGFloat)y {
    CGRect newBytesHeaderFrame = self.bytesResultHeader.frame;
    newBytesHeaderFrame.origin.y = y + 12;
    self.bytesResultHeader.frame = newBytesHeaderFrame;
    
    CGRect newBytesDelimiterFrame = self.bytesResultDelimiter.frame;
    newBytesDelimiterFrame.origin.y = y + 36;
    self.bytesResultDelimiter.frame = newBytesDelimiterFrame;
    
    CGRect newBytesValueFrame = self.bytesResultLabel.frame;
    newBytesValueFrame.origin.y = y + 48;
    self.bytesResultLabel.frame = newBytesValueFrame;
}

- (void)setTextResult:(NSString*)text {
    self.textResultLabel.text = text;
    
    CGSize maximumLabelSize = CGSizeMake(self.textResultLabel.bounds.size.width, FLT_MAX);
    
    CGSize expectedLabelSize = [text sizeWithFont:self.textResultLabel.font
                                constrainedToSize:maximumLabelSize
                                    lineBreakMode:self.textResultLabel.lineBreakMode];
    
    //adjust the label the the new height.
    CGRect newFrame = self.textResultLabel.frame;
    newFrame.size.height = expectedLabelSize.height;
    self.textResultLabel.frame = newFrame;
    
    [self adjustBytesLabels:newFrame.origin.y + newFrame.size.height];
}

- (void)setBytesResult:(NSString*)text {
    self.bytesResultLabel.text = text;
    
    CGSize maximumLabelSize = CGSizeMake(self.bytesResultLabel.bounds.size.width, FLT_MAX);
    
    CGSize expectedLabelSize = [text sizeWithFont:self.bytesResultLabel.font
                                constrainedToSize:maximumLabelSize
                                    lineBreakMode:self.bytesResultLabel.lineBreakMode];
    
    //adjust the label the the new height.
    CGRect newFrame = self.bytesResultLabel.frame;
    newFrame.size.height = expectedLabelSize.height;
    self.bytesResultLabel.frame = newFrame;
    
    self.scrollView.contentSize = CGSizeMake(self.scrollView.bounds.size.width, newFrame.size.height + newFrame.origin.y);
}

- (void)reloadLabels {
    if ([self scanResult] != nil) {
        [self setTextResult:self.scanResult.text];
        [self setBytesResult:self.scanResult.bytes];
    } else {
        self.textResultLabel.text = @"No scanning result";
        self.bytesResultLabel.text = @"No scanning result";
    }
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    
    [self reloadLabels];
}

- (void)viewWillLayoutSubviews {
    [super viewWillLayoutSubviews];
    
    [self reloadLabels];
}

@end
