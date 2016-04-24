#import "ViewController.h"
#include <Bibim/GameFramework.h>
#include <Bibim/GameModuleTree.h>
#include <Bibim/GameWindow.h>
#include <Bibim/IME.h>
#include <Bibim/BGM.h>
#include <Bibim/SoundFX.h>
#include <Bibim/Environment.h>
#include <Bibim/Point2.h>
using namespace Bibim;

@interface ViewController () {
    GameFramework* game;
}
@property (strong, nonatomic) EAGLContext* context;
@end

@implementation ViewController

- (Point2)screenSizeInPixels {
    const UIScreen* screen = [UIScreen mainScreen];
    const CGRect screenBounds = [screen bounds];
    const float scale = [screen scale];
    
    UIInterfaceOrientation orientation = [UIApplication sharedApplication].statusBarOrientation;
    if (([[[UIDevice currentDevice] systemVersion] floatValue] < 8.0f) && UIInterfaceOrientationIsLandscape(orientation))
    {
        return Point2((int)(screenBounds.size.height * scale),
                      (int)(screenBounds.size.width * scale));
    }
    else
    {
        return Point2((int)(screenBounds.size.width * scale),
                      (int)(screenBounds.size.height * scale));
    }
}

- (Point2)touchPoint:(UITouch*)touch {
    CGPoint point = [touch locationInView:self.view];
    const float scale = [[UIScreen mainScreen] scale];
    return Point2((int)(point.x * scale), (int)(point.y * scale));
}

- (void)viewDidLoad {
    [super viewDidLoad];

    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyBundleURL(mainBundle);
    CFStringRef workingDirectoryString = CFURLCopyFileSystemPath(resourcesURL, kCFURLPOSIXPathStyle);
    char workingDirectory[PATH_MAX];
    CFStringGetCString(workingDirectoryString, workingDirectory, PATH_MAX, kCFStringEncodingUTF8);
    CFRelease(workingDirectoryString);
    const int length = static_cast<int>(strlen(workingDirectory));
    workingDirectory[length] = '/';
    workingDirectory[length + 1] = '\0';
    
    NSArray* documentPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString* documentDirectory = [documentPaths objectAtIndex:0];
    
    Environment::Setup("ko-KR", workingDirectory, [documentDirectory UTF8String]);
    
    bgmProxy = [[BBBGM alloc] init];
    sfxProxy = [[BBSoundFX alloc] init];
    BGM::SetProxy((__bridge void*)bgmProxy);
    SoundFX::SetProxy((__bridge void*)sfxProxy);
    
    game = GameFramework::Create();
    
    GLKView* view = (GLKView*)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormatNone;
    
    [EAGLContext setCurrentContext:self.context];
    
    game->GetWindow()->SetSize([self screenSizeInPixels]);
    game->init();
}

- (void)dealloc {
    [EAGLContext setCurrentContext:self.context];
    
    if (game) {
        // Finalize Game
    }
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
    
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - update & draw

- (void)update {
    game->update();
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    game->draw();
}

#pragma mark - input

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    Point2 xy = [self touchPoint:[touches anyObject]];
    game->GetWindow()->RaiseMouseLeftButtonDownEvent(xy.X, xy.Y);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    Point2 xy = [self touchPoint:[touches anyObject]];
    game->GetWindow()->RaiseMouseLeftButtonUpEvent(xy.X, xy.Y);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    Point2 xy = [self touchPoint:[touches anyObject]];
    game->GetWindow()->RaiseMouseMoveEvent(xy.X, xy.Y);
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    Point2 xy = [self touchPoint:[touches anyObject]];
    game->GetWindow()->RaiseMouseLeftButtonUpEvent(xy.X, xy.Y);
}
@end
