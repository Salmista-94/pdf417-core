//
//  PPRootViewController.m
//  pdf417-core-demo
//
//  Created by Jura on 06/12/13.
//  Copyright (c) 2013 PhotoPay. All rights reserved.
//

#import "PPRootViewController.h"
#import "PPScanManager.h"
#import "PPResultViewController.h"
#import <MobileCoreServices/MobileCoreServices.h>

#define IS_IPAD (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)

@interface PPRootViewController () <UINavigationControllerDelegate, UIImagePickerControllerDelegate>

@property (nonatomic, strong) UIPopoverController *popoverViewController;

@end

@implementation PPRootViewController

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
    
    self.title = @"pdf417.mobi";
    
    
    // Add help button to navigation bar
    UIBarButtonItem *scanButton = [[UIBarButtonItem alloc] initWithTitle:@"Scan"
                                                                   style:UIBarButtonItemStyleBordered
                                                                  target:self
                                                                  action:@selector(scanImage)];
    self.navigationItem.rightBarButtonItem = scanButton;
    
    UIBarButtonItem *photoButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemCamera target:self action:@selector(cameraPressed)];
    self.navigationItem.leftBarButtonItem = photoButton;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Button callbacks

- (void)scanImage {
    PPScanResult* scanResult = [PPScanManager scanImage:[self imageView].image];
    
    PPResultViewController *resultViewContreller = [[PPResultViewController alloc] initWithNibName:@"PPResultViewController" bundle:nil];
    [resultViewContreller setScanResult:scanResult];

    [[self navigationController] pushViewController:resultViewContreller animated:YES];
}

- (void)cameraPressed {
    if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera] == NO) {
        [self openPhotos];
    } else {
        [self takePhoto];
    }
}

- (void)takePhoto {
    UIImagePickerController *cameraUI = [[UIImagePickerController alloc] init];
    
    // Use rear camera
    cameraUI.sourceType = UIImagePickerControllerSourceTypeCamera;
    cameraUI.cameraDevice = UIImagePickerControllerCameraDeviceRear;
    
    // Displays a control that allows the user to choose only photos
    cameraUI.mediaTypes = [[NSArray alloc] initWithObjects: (NSString *)kUTTypeImage, nil];
    
    // Hides the controls for moving & scaling pictures, or for trimming movies.
    cameraUI.allowsEditing = NO;
    
    // Shows default camera control overlay over camera preview.
    // TODO: set this to NO and provide custom overlay
    cameraUI.showsCameraControls = YES;
    
    // set delegate
    cameraUI.delegate = self;
    
    // Show view
    // in iOS7 (as of DP6) this shows a bugged status bar (see https://devforums.apple.com/message/861462#861462)
    // TODO: iOS 6 should be tested
    // iOS5 works OK, just like Facebook app
    [self presentViewController:cameraUI animated:YES completion:nil];
}

- (void)openPhotos {
    UIImagePickerController *cameraUI = [[UIImagePickerController alloc] init];
    
    // Use rear camera
    cameraUI.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    
    // Displays a control that allows the user to choose only photos
    cameraUI.mediaTypes = [[NSArray alloc] initWithObjects: (NSString *)kUTTypeImage, nil];
    
    // Hides the controls for moving & scaling pictures, or for trimming movies.
    cameraUI.allowsEditing = NO;
    
    // set delegate
    cameraUI.delegate = self;
    
    // Show view
    // in iOS7 (as of DP6) this shows a bugged status bar (see https://devforums.apple.com/message/861462#861462)
    // TODO: iOS 6 should be tested
    // iOS5 works OK, just like Facebook app
    
    if (IS_IPAD) {
        CGRect rect = self.navigationController.navigationBar.frame;
        rect.size.width = 88.0f;
//        rect.size.height = rect.size.height / 2;
        UIPopoverController *popover = [[UIPopoverController alloc] initWithContentViewController:cameraUI];
        [popover presentPopoverFromRect:rect inView:self.view permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
        _popoverViewController = popover;
    } else {
        [self presentViewController:cameraUI animated:YES completion:nil];
    }
}

#pragma mark - UIImagePickerControllerDelegate

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info {
    NSString *mediaType = [info objectForKey: UIImagePickerControllerMediaType];
    
    // Handle a still image capture
    if (CFStringCompare((CFStringRef) mediaType, kUTTypeImage, 0) == kCFCompareEqualTo) {
        UIImage *image = (UIImage *) [info objectForKey: UIImagePickerControllerOriginalImage];
        
        self.imageView.image = image;
        
    }
    [self dismissViewControllerAnimated:YES completion:nil];
}


@end
