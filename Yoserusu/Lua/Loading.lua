require( "Lua/Enum"  );

function Loading()
    local this = {}

    this.mBlack = SpriteUtil();
    this.mBackGraound = SpriteUtil( "res/image/bg.png" );

    this.mFade = 0;
    
    this.draw = function( self )
        local switch = {}

        trance = math.sin( self.mFade / 30 ) / 2 + 0.5;

        --背景描画
        self.mBlack:setColor( 0.0, 0.0, 0.0 );
        self.mBlack:setTransparency( trance );
        self.mBlack:drawRectangle( 0, 0, 1024, 800 );
        
        self.mBackGraound:draw( 0, 0 );

        self.mFade = self.mFade + 1;

        return self.mFade;
        
    end

    return this;

end