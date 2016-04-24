#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#include <Bibim/../../src/Audio/BGM-ObjectiveC.h>
#include <Bibim/../../src/Audio/SoundFX-ObjectiveC.h>

@interface ViewController : GLKViewController
{
    BBBGM* bgmProxy;
    BBSoundFX* sfxProxy;
}

@end

