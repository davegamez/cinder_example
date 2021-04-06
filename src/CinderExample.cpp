//
//  CinderExample.cpp
//  CinderExample
//
//  Created by Dave Gamez on 9/9/12.
//  Copyright (c) 2012 Dave Gamez. All rights reserved.
//

#include "cinder/app/AppBasic.h"
#include "cinder/ImageIO.h"
#include "cinder/gl/Texture.h"
#include "cinder/Perlin.h"
#include "cinder/Channel.h"
#include "cinder/Vector.h"
#include "cinder/Utilities.h"
#include "ParticleController.h"

#define RESOLUTION 10
#define NUM_PARTICLES_TO_SPAWN 15

#include <sstream>
using std::stringstream;

using namespace ci;
using namespace ci::app;

class CinderExample : public AppBasic {
public:
	void prepareSettings( Settings *settings );
	void keyDown( KeyEvent event );
	void mouseDown( MouseEvent event );
	void mouseUp( MouseEvent event );
	void mouseMove( MouseEvent event );
	void mouseDrag( MouseEvent event );
	void setup();
	void update();
	void draw();
	
	Perlin mPerlin;
    
	Channel32f mChannel;
	gl::Texture	mTexture;
	
	Vec2i mMouseLoc;
	Vec2f mMouseVel;
	bool mIsPressed;
	
	ParticleController mParticleController;
	
	bool mDrawParticles;
	bool mDrawImage;
	bool mSaveFrames;
	bool mCentralGravity;
	bool mAllowPerlin;
	
	int mSaveFrameCount;
};

void CinderExample::prepareSettings( Settings *settings )
{
	settings->setWindowSize( 800, 800 );
	settings->setFrameRate( 60.0f );
}

void CinderExample::setup()
{
	mPerlin = Perlin();
	
    mChannel = Channel32f( loadImage( loadResource( "background.tiff" ) ) );
	mTexture = mChannel;
    
	mMouseLoc = Vec2i( 0, 0 );
	mMouseVel = Vec2f::zero();
	
	mDrawParticles	= true;
	mDrawImage		= false;
	mIsPressed		= false;
	mCentralGravity = false;
	mAllowPerlin	= false;
	mSaveFrames		= false;
	mSaveFrameCount = 0;
}


void CinderExample::mouseDown( MouseEvent event )
{
	mIsPressed = true;
}

void CinderExample::mouseUp( MouseEvent event )
{
	mIsPressed = false;
}

void CinderExample::mouseMove( MouseEvent event )
{
	mMouseVel = ( event.getPos() - mMouseLoc );
	mMouseLoc = event.getPos();
}

void CinderExample::mouseDrag( MouseEvent event )
{
	mouseMove( event );
}

void CinderExample::keyDown( KeyEvent event )
{
	if( event.getChar() == '1' ){
		mDrawImage = ! mDrawImage;
	} else if( event.getChar() == '2' ){
		mDrawParticles = ! mDrawParticles;
	}
	
	if( event.getChar() == 's' ){
		mSaveFrames = ! mSaveFrames;
	} else if( event.getChar() == 'g' ){
		mCentralGravity = ! mCentralGravity;
	} else if( event.getChar() == 'p' ){
		mAllowPerlin = ! mAllowPerlin;
	}else if( event.getChar() == '3'){
        writeImage( getHomeDirectory().string() + "Desktop/renders/images/image_" + toString( getElapsedFrames() ) + ".png",copyWindowSurface());
    }
}


void CinderExample::update()
{
	if( ! mChannel ) return;
	
	if( mIsPressed )
		mParticleController.addParticles( NUM_PARTICLES_TO_SPAWN, mMouseLoc, mMouseVel );
	
	mParticleController.repulseParticles();
	
	if( mCentralGravity )
		mParticleController.pullToCenter();
    
	if( mAllowPerlin )
		mParticleController.applyPerlin( mPerlin );
    
	mParticleController.update( mChannel, mMouseLoc );
}

void CinderExample::draw()
{
	gl::clear( Color( 0, 0, 0 ), true );
	
	if( mDrawImage ){
		mTexture.enableAndBind();
		gl::draw( mTexture, getWindowBounds() );
	}
	
	if( mDrawParticles ){
		glDisable( GL_TEXTURE_2D );
		mParticleController.draw();
	}
	
	if( mSaveFrames ){
        writeImage( getHomeDirectory().string() + "Desktop/renders/image_" + toString( getElapsedFrames() ) + ".png",copyWindowSurface());

	}
}

CINDER_APP_BASIC( CinderExample, RendererGl )
