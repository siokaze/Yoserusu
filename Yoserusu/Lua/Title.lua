require( "Lua/Enum"  );

function Title()
    local this = {}

    this.mBackGraound = SpriteUtil( "res/image/bg.png" );

    this.mTitleBitmap = SpriteUtil( "res/image/titile.png" );
    this.mStart = SpriteUtil( "res/image/start4.png" );

    this.mBall = Model( "res/model/Ball.pmd" );

    this.mBallPose = Vector3( 0.0, 0.0 );
    
    this.draw = function( self )
        local switch = {}

        --背景描画
        self.mBackGraound:draw( 0, 0 );

        --タイトル描画
        self.mTitleBitmap:draw( 90, -10 );

        --ボール描画
        --self.mBall:setPosition(mBallPos);
        --self.mBall:setColor(Vector3(1,1,1));
        --self.mBall:draw( DrawType.TYPE_BALL );

        --スタートアップ描画
        self.mStart:draw( 280, 550 );

        return 0;
        
    end

    return this;

end