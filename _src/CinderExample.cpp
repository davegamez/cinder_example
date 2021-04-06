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

#define RESOLUTION 5

using namespace ci;
using namespace ci::app;

class CinderExample : public AppBasic {
public:
	void prepareSettings( Settings *settings );
	void keyDown( KeyEvent event );
	void mouseMove( MouseEvent event );
	void mouseDrag( MouseEvent event );
	void setup();
	void update();
	void draw();
	
	Channel32f mChannel;
	gl::Texture	mTexture;
	
	Vec2i mMouseLoc;
	
	ParticleController mParticleController;
	
	bool mDrawParticles;
	bool mDrawImage;
};

void CinderExample::prepareSettings( Settings *settings )
{
	settings->setWindowSize( 800, 800 );
	settings->setFrameRate( 60.0f );
}

void CinderExample::mouseMove( MouseEvent event )
{
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
	} else if( event.getChar() == '3'){
        //writeImage("image_frame#.png",copyWindowSurface());
        //writeImage( getHomeDirectory() / "image_" / toString( getElapsedFrames() ) / ".png", copyWindowSurface() );
        writeImage( getHomeDirectory().string() + "Desktop/renders/image_" + toString( getElapsedFrames() ) + ".png",copyWindowSurface());
    }
}

void CinderExample::setup()
{
	//Url url( "http://www.libcinder.org/media/tutorial/paris.jpg" );
	//mChannel = Channel32f( loadImage( loadUrl( url ) ) );
    mChannel = Channel32f( loadImage( loadResource( "background.tiff" ) ) );
	mTexture = mChannel;
    
	mParticleController = ParticleController( RESOLUTION );
	
	mMouseLoc = Vec2i( 0, 0 );
	
	mDrawParticles = true;
	mDrawImage = false;
}

void CinderExample::update()
{
	if( ! mChannel ) return;
	
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
}

CINDER_APP_BASIC( CinderExample, RendererGl )
