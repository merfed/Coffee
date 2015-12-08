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
//  BLPImageRep.m
//  Blipper
//

#import "BLPImageRep.h"
#import <squish/squish.h>
#import "BLPCore.h"

using namespace BLPCore;

@implementation NSData (BLPImageAdditions)

-(DataChunk)dataChunk
{
    return DataChunk((uint8_t*)[self bytes], (uint8_t*)[self bytes]+[self length]);
}

+(NSData *)dataWithDataChunk:(DataChunk)chunk
{
    return [NSData dataWithBytes:chunk.data() length:chunk.size()];
}

@end

@implementation BLPImageRep
{
    BLPHeader fileHeader;
    NSInteger dataSize;
    NSData    *pixelData;
    NSMutableArray *colorPalette;
}

+ (void)load
{
    [NSImageRep registerImageRepClass:self];
    NSLog(@"loaded BLP image rep");
}

+ (NSArray *)imageUnfilteredTypes
{
	return @[@"com.blizzard.blp"];
}

+(NSArray *)imageFileTypes
{
    return @[@"blp"];
}

+(Class)imageRepClassForType:(NSString *)type
{
    return [type isEqualToString:@"com.blizzard.blp"] ? [BLPImageRep class] : nil;
}

+(Class)imageRepClassForData:(NSData *)data
{
    return [BLPImageRep canInitWithData:data] ? [BLPImageRep class] : nil;
}

+(Class)imageRepClassForFileType:(NSString *)type
{
    return [type isEqualToString:@"blp"] ? [BLPImageRep class] : nil;
}

+ (BOOL)canInitWithData:(NSData *)data
{
    DataChunk dataChunk([data dataChunk]);
    
	return BLPCore::CheckBLPMagic(dataChunk);
}

+ (NSArray *) imageUnfilteredPasteboardTypes
{
	return nil;
}

- (id)initWithData:(NSData *)data
{
    DataChunk fileData([data dataChunk]);
    fileHeader = BLPCore::GetBLPHeader(fileData);
    
    DataChunk pixels;
    
    if (!BLPCore::DecodeBLPData(fileData, pixels)) {
        return nil;
    }
    
    if (!pixels.empty()) {
        pixelData = [NSData dataWithDataChunk:pixels];
        [super setSize:NSMakeSize(fileHeader.width, fileHeader.height)];
        [super setPixelsHigh:fileHeader.height];
        [super setPixelsWide:fileHeader.width];
        [super setBitsPerSample:32];
        [super setAlpha:YES];
    }
    
    return self;
}

+ (id)imageRepWithData:(NSData *)data
{
	if (data == nil)
    {
		return nil;
    }
	
	return [[BLPImageRep alloc] initWithData: data];
}

+ (id)imageRepWithContentsOfURL:(NSURL *)url
{
    if (url == nil) {
        return nil;
    }
    
    NSData *rawData = [NSData dataWithContentsOfURL:url];
    return [[BLPImageRep alloc] initWithData:rawData];
}

+(id)imageRepWithContentsOfFile:(NSString *)filename
{
    if (filename == nil) {
        return nil;
    }
    
    NSData *rawData = [NSData dataWithContentsOfFile:filename];
    return [[BLPImageRep alloc] initWithData:rawData];
}

-(CGImageRef)CGImage
{
    if (!pixelData) {
        NSLog(@"No pixel data found! Can't draw!");
        return NULL;
    }
    
    CGDataProviderRef provider = CGDataProviderCreateWithCFData((__bridge CFDataRef)pixelData);
    CGColorSpaceRef colorspace = CGColorSpaceCreateDeviceRGB();
    CGImageRef image = NULL;
    
    image = CGImageCreate(fileHeader.width*1.0f, fileHeader.height*1.0f, 8, 32, sizeof(RGBAPixel)*fileHeader.width,
                          colorspace, kCGBitmapByteOrderDefault  | kCGImageAlphaLast,
                          provider, NULL, false, kCGRenderingIntentDefault);
    
    CGColorSpaceRelease(colorspace);
    CGDataProviderRelease(provider);
    
    return image;
}

-(BOOL)draw
{
    CGRect rect = CGRectMake(0, 0, fileHeader.width, fileHeader.height);
    
    [self drawInRect:rect];
    
    return YES;
}

-(BOOL)drawInRect:(NSRect)rect
{
    CGImageRef image = [self CGImage];
    
    if (image) {
        CGContextRef context = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
        CGContextDrawImage(context, rect, image);
        CGImageRelease(image);
        
        return YES;
    }
    
    return NO;
}

+(NSData *)BLPWithData:(NSData *)data type:(BLPFormat)type size:(NSSize)size
{
    if (type == BLPFormatPlain) {
        DataChunk inData = [data dataChunk];
        DataChunk outData = EncodePlainBLP(inData, size.width, size.height);
        
        return [NSData dataWithDataChunk:outData];
    }
    
    return nil;
}

@end