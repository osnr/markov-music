//
//  AppDelegate.m
//  Markov Brothers
//
//  Created by Jan Dlabal on 10/13/12.
//  Copyright (c) 2012 Jan Dlabal. All rights reserved.
//

#import "AppDelegate.h"

@implementation AppDelegate
@synthesize selectedFiles;
@synthesize inputField;
@synthesize outputField;
@synthesize fuzz;
@synthesize order;
@synthesize unify;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
    fileNames = nil;
}

- (void)play:(id)sender {
    system("mplayer /Users/jan/devel/markov-music/out.wav &> /dev/null&");
}

- (void)run:(id)sender {
    NSMutableString *cmdLine = [[NSMutableString alloc] initWithString:@"xterm -e \"cd  /Users/jan/devel/markov-music/; rm SavedMagicModel/*; /Users/jan/devel/markov-music/markov-music "];
    [cmdLine appendFormat:@"%d ", [inputField intValue] * 44100];
    [cmdLine appendFormat:@"%d ", [outputField intValue] * 44100];
    [cmdLine appendFormat:@"%@ ", [fuzz stringValue]];
    [cmdLine appendFormat:@"%@ ", [order stringValue]];
    [cmdLine appendFormat:@"%s ", [unify state] == NSOnState ? "1" : "0"];
    for (int i = 0; i < [fileNames count]; i++) {
        [cmdLine appendFormat:@"\"%@\" ", [fileNames objectAtIndex:i]];
    }
    [cmdLine appendString:@"; read\""];
    NSLog(@"Running command : %@\n", cmdLine);
    system([cmdLine UTF8String]);
    [self play:self];
}

- (void)selectInput:(id)sender {
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    [panel setAllowsMultipleSelection:YES];
    [panel runModal];
    fileNames = [panel filenames];
    [selectedFiles setStringValue:[NSString stringWithFormat:@"%li files selected", [fileNames count]]];
}

@end
