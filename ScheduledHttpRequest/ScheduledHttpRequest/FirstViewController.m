//
//  FirstViewController.m
//  ScheduledHttpRequest
//
//  Created by HAMIDOUCHE Lyes on 02/10/2015.
//  Copyright © 2015 HAMIDOUCHE Lyes. All rights reserved.
//

#import "FirstViewController.h"

#define ADDR @"172.16.0.1"
#define PORT 12345
#define RECONNECT_WAITING_TIME 5.0

// Private methods and properties
@interface FirstViewController ()
    @property NSInteger rank; // Represents the rank of the iPad in the iPads set
    @property BOOL conneced; // Value set to true when the device is connected to the device
@end

@implementation FirstViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    //    DownloadControler *downloadController = [[DownloadControler alloc] init];
    

    
    self.downloadController = [[DownloadControler alloc] init];
    //Example String
    NSDate *startDate= [[NSDate date] dateByAddingTimeInterval:1.0];
    //MyCustomRequest * request =  [self.downloadController createRequestWithURL:@"http://localhost:8000/0-0/Video.mp4" andStartDate:startDate];
    [self.downloadController createRequestWithURL:@"http://files.magency.fr/SyncVideoContents.zip" andStartDate:startDate];
    self.asyncSocket  = [[AsyncSocket alloc] initWithDelegate:self];
    NSError *error;
    self.rank = -1;
    //[self.asyncSocket connectToHost:ADDR onPort:PORT error:&error];

}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma AsyncSocketDelegate
// socket managed to connect to the server
- (void)onSocket:(AsyncSocket *)sock didConnectToHost:(NSString *)host port:(UInt16)port {
    NSLog(@"The iPad is connected to %@ at port %d",host,port);
    [sock readDataWithTimeout:-1 tag:0];
}

// socket will disconnect from the server
- (void)onSocket:(AsyncSocket *)sock willDisconnectWithError:(NSError *)err {
    NSLog(@"The socket is going to disconnect, the reason %@",err.description);
    // Save data for future use.
    
}

// socket did disconnect from or could not connect to the server
- (void) onSocketDidDisconnect:(AsyncSocket *)sock {
    // Force the device to reconnect each 5 seconds when disconnected
    [NSTimer scheduledTimerWithTimeInterval:RECONNECT_WAITING_TIME target:self selector:@selector(reconnect:) userInfo:Nil repeats:NO];
    
}


// socket did read data
- (void)onSocket:(AsyncSocket *)sock didReadData:(NSData *)data withTag:(long)tag
{
    NSString *string = [NSString stringWithUTF8String:data.bytes];
    string = [string stringByReplacingOccurrencesOfString:@"\n" withString:@""];
    NSArray  *substrings = [string componentsSeparatedByString:@":"];

    if (substrings) {

        if ([substrings count]>=1) {
            NSString * command = substrings[0];
    
            if ([command isEqualToString:@"Download"]) {
                
                // Download immediately the specified data
                [self.downloadController createRequestWithURL:substrings[1]];

            } else if ([command isEqualToString:@"DownloadAt"]) {
            
                // Download the specified data in X seconds
                NSDate *now,*start;
                now = [NSDate date];
                NSTimeInterval timeInterval = (NSTimeInterval) [substrings[2] doubleValue];
                start  = [now dateByAddingTimeInterval:timeInterval];
                [self.downloadController createRequestWithURL:substrings[1] andStartDate:start];
            
            } else if ([command isEqualToString:@"Rank"]) {
                
                self.rank = [substrings[1] integerValue];
            
            }
        }
    }
    [sock readDataWithTimeout:-1 tag:0];
}


// socket did write data
- (void) onSocket:(AsyncSocket *)sock didWriteDataWithTag:(long)tag {
    [sock readDataWithTimeout:-1 tag:0];
}

#pragma mark reconnection handler

- (void) reconnect:(id) sender {
    self.asyncSocket  = [[AsyncSocket alloc] initWithDelegate:self];
    NSError *error;
    [self.asyncSocket connectToHost:ADDR onPort:PORT error:&error];
}

@end


// Remove it after that
// Do any additional setup after loading the view, typically from a nib.
//    DownloadControler *downloadController = [[DownloadControler alloc] init];
//    NSDate *startDate= [[NSDate date] dateByAddingTimeInterval:5.0];
//    MyCustomRequest * request =  [downloadController createRequestWithURL:@"http://localhost:8000/0-0/Video.mp4" andStartDate:startDate];
//
//    NSLog(@"Request.url");