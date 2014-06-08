require( "Lua/Enum"  );

function Result()
    local this = {}

    this.mBall = ModelObj( "res/model/Ball.pmd" );

    this.mStrTex = SpriteUtil("res/image/Ending.png");
    this.mBackGround = SpriteUtil( "res/image/Ending_bg.png");

    this.mBallPos = Vector3( 0.0, -20.0, 0.0 );
    this.mBallAng = Vector3( 0.0, 0.0, 0.0 );
    this.mBallScale = Vector3( 0.5, 0.5, 0.5 );
    
    this.draw = function( self )
        local switch = {}

        self.mBallAng.y = self.mBallAng.y + 3.0;

        if self.mBallPos.y < 20 then
            self.mBallPos.y = self.mBallPos.y + 0.1 * 1.5;
        end

        self.mBackGround:setTransparency( 1.0 );
        self.mBackGround:draw( 0, 0 );

        --ボール
        self.mBall:setPosition( self.mBallPos );
        self.mBall:setScale( self.mBallScale );
        self.mBall:setAngle( self.mBallAng );
        self.mBall:draw( DrawType.TYPE_BALL );

        self.mStrTex:draw( 0, 0 );

        return self.mBallPos.y;
        
    end

    return this;

end