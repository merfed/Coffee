// Copyright (c) 2014 Norgannon
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//  BPDocument.m
//  Blipper
//

#import "BPDocument.h"
#import "BLPImageRep.h"

@implementation BPDocument

- (id)init
{
    self = [super init];
    if (self) {
        // Add your subclass-specific initialization here.
    }
    return self;
}

- (NSString *)windowNibName
{
    // Override returning the nib file name of the document
    // If you need to use a subclass of NSWindowController or if your document supports multiple NSWindowControllers, you should remove this method and override -makeWindowControllers instead.
    return @"BPDocument";
}

- (void)windowControllerDidLoadNib:(NSWindowController *)aController
{
    [super windowControllerDidLoadNib:aController];
    // Add any code here that needs to be executed once the windowController has loaded the document's window.
    
    float imageWidth = CGImageGetWidth(self.BLPImage);
    float imageHeight = CGImageGetHeight(self.BLPImage);
    
    [self.imageView setImage:self.BLPImage imageProperties:NULL];
    
    [self.sizeField setStringValue:[NSString stringWithFormat:@"%.0fx%.0f", imageWidth, imageHeight]];
    
    float newWindowWidth;
    float newWindowHeight;
    
    if (imageWidth > 360 && imageWidth <= 1024) {
        newWindowWidth = imageWidth;
    } else if (imageWidth > 1024) {
        newWindowWidth = 1024;
    } else {
        newWindowWidth = 360;
    }
    
    if (imageHeight <= 1024) {
        newWindowHeight = imageHeight+77;
    } else {
        newWindowHeight = 728;
    }
    
    NSRect newWindowFrame = NSMakeRect(self.docWindow.frame.origin.x,
                                       self.docWindow.frame.origin.y,
                                       newWindowWidth, newWindowHeight);
    [self.docWindow setFrame:newWindowFrame display:YES];
}

+ (BOOL)autosavesInPlace
{
    return NO;
}

- (NSData *)dataOfType:(NSString *)typeName error:(NSError **)outError
{
    // Insert code here to write your document to data of the specified type. If outError != NULL, ensure that you create and set an appropriate error when returning nil.
    // You can also choose to override -fileWrapperOfType:error:, -writeToURL:ofType:error:, or -writeToURL:ofType:forSaveOperation:originalContentsURL:error: instead.
  /*  NSData *rawData = (__bridge NSData *)CGDataProviderCopyData(CGImageGetDataProvider(self.imageView.image));
    
    return [BLPImageRep BLPWithData:rawData type:BLPFormatPlain
                               size:NSMakeSize(CGImageGetWidth(self.imageView.image), CGImageGetHeight(self.imageView.image))];*/
    NSException *exception = [NSException exceptionWithName:@"UnimplementedMethod" reason:[NSString stringWithFormat:@"%@ is unimplemented", NSStringFromSelector(_cmd)] userInfo:nil];
    @throw exception;
    
    return nil;
}

- (BOOL)readFromData:(NSData *)data ofType:(NSString *)typeName error:(NSError **)outError
{
    if ([BLPImageRep canInitWithData:data]) {
        BLPImageRep *BLPImage = [BLPImageRep imageRepWithData:data];
        self.BLPImage = [BLPImage CGImage];
        
        return YES;
    }
    
    
    return NO;
}

- (IBAction)changeSelectedTool:(id)sender {
    NSString *chosenTool;
    
    switch ([sender selectedSegment]) {
        case 0:
            chosenTool = @"IKToolModeNone";
            break;
        
        case 1:
            chosenTool = @"IKToolModeMove";
            break;
            
        case 2:
            chosenTool = @"IKToolModeSelect";
            break;
            
        case 3:
            chosenTool = @"IKToolModeSelectEllipse";
            break;
            
        case 4:
            chosenTool = @"IKToolModeSelectLasso";
            break;
            
        case 5:
            chosenTool = @"IKToolModeRotate";
            break;
            
        default: chosenTool = @"IKToolModeNone";
            break;
    }
    
    [self.imageView setCurrentToolMode:chosenTool];
    
}

-(void)close
{
    NSLog(@"Deallocating");
    [self.imageView setImage:NULL imageProperties:NULL];
    self.BLPImage = NULL;
    CGImageRelease(self.BLPImage);
    
    [super close];
}

- (IBAction)changeZoom:(id)sender {
    NSLog(@"%@", self.BLPImage);
    if ([sender selectedSegment] == 0) {
        [self.imageView zoomOut:nil];
    } else {
        [self.imageView zoomIn:nil];
    }
}

- (IBAction)changeRotation:(id)sender {
    if ([sender selectedSegment] == 0) {
        [self.imageView rotateImageLeft:nil];
    } else {
        [self.imageView rotateImageRight:nil];
    }
}

-(void)showExportSheet:(id)sender
{
    NSSavePanel *exportPanel = [NSSavePanel savePanel];
    
    [exportPanel setAccessoryView:self.formatView];
    [exportPanel setCanCreateDirectories:YES];
    [exportPanel setPrompt:@"Export"];
    
    [exportPanel beginSheetModalForWindow:self.docWindow completionHandler:^(NSInteger result) {
        if (result == NSFileHandlingPanelOKButton) {
            [self exportImageToURL:[exportPanel URL]];
        }
    }];
    
}

-(void)saveBLP:(NSURL *)url
{
    NSData *rawData = (__bridge NSData *)CGDataProviderCopyData(CGImageGetDataProvider(self.imageView.image));
    
    [BLPImageRep BLPWithData:rawData type:BLPFormatPlain
                        size:NSMakeSize(CGImageGetWidth(self.imageView.image), CGImageGetHeight(self.imageView.image))];
}

-(void)exportImageToURL:(NSURL *)url
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSInteger fileType;
    NSDictionary *options = nil;
    
    switch ([defaults integerForKey:@"selectedExportFormat"]) {
        case 0:
            fileType = NSJPEGFileType;
            options = @{NSImageCompressionFactor:[NSNumber numberWithInteger:[defaults integerForKey:@"exportQuality"]]};
            break;
        
        case 1:
            fileType = NSPNGFileType;

            break;
            
        case 2:
            fileType = NSTIFFFileType;
            break;
            
        default:
            break;
    }
    
    NSImage *image = [[NSImage alloc] initWithCGImage:self.imageView.image
                                                 size:NSZeroSize];
    NSData *imageData = [image TIFFRepresentation];
    NSBitmapImageRep *imageRep = [NSBitmapImageRep imageRepWithData:imageData];
    imageData = [imageRep representationUsingType:fileType properties:options];
    [imageData writeToURL:url atomically:NO];
}

@end
