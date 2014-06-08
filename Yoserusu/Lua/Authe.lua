
require( "Lua/Enum"  );

function Authe()
    local this = {}

    this.mLeftHand = SpriteUtil();
    this.mRightHand = SpriteUtil();

    this.mAuth = SpriteUtil( "res/image/Auth.png" );
    this.mOK = SpriteUtil( "res/image/Ok.png" );
    this.mRedMaru = SpriteUtil( "res/image/RedMaru.png" );
    this.mBlueMaru = SpriteUtil( "res/image/BlueMaru.png" );  
    this.mTitle_Auth = SpriteUtil( "res/image/Title_Auth.png");
    this.mBackGraound = SpriteUtil( "res/image/bg.png" );
    this.mHandCheckCount = 0;
    
    this.draw = function( self, mMoveSceneFlag, check )
        local switch = {}

        --îwåiÇÃï`âÊ
        self.mBackGraound:draw( 0, 0 );

        --îFèÿîÕàÕâÊëú
        self.mRedMaru:draw( 600, 100 );
        self.mBlueMaru:draw( 200, 100 );

        --switchÉPÅ[ÉX 
        switch[ Mode.MODE_WAIT ] = function ()
            self.mTitle_Auth:draw( 130, 400 );
        end
        switch[ Mode.MODE_NOW ] = function ()
           self.mTitle_Auth:draw( 130, 400 );
           if ( check ) then
                self.mAuth:draw( 200, 530 );
            end
        end
        switch[ Mode.MODE_END ] = function ()
            self.mOK:draw( 130, 530 );
        end

        switch[ mMoveSceneFlag ]();

        return 0;
        
    end

    this.handDraw = function( self, rX, rY, lX, lY )
        --éËÇÃï`âÊ

        red = Vector3( 0.0, 0.0, 1.0 );

        self.mLeftHand:setColor( red );
        self.mLeftHand:drawEllipse( lX, lY, 30, 30 );

        self.mRightHand:setColor( 1.0, 0.0, 0.0 );
        self.mRightHand:drawEllipse( rX, rY, 30, 30 );

        return 0;
    end

    return this;

end