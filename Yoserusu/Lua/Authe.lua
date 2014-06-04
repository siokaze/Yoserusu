require( "Lua/Enum"  );

function Authe()
    local this = {}

    this.mAuth = SpriteUtil( "res/image/Auth.png" );
    this.mOK = SpriteUtil( "res/image/Ok.png" );
    this.mRedMaru = SpriteUtil( "res/image/RedMaru.png" );
    this.mBlueMaru = SpriteUtil( "res/image/BlueMaru.png" );  
    this.mTitle_Auth = SpriteUtil( "res/image/Title_Auth.png");
    this.mHandCheckCount = 0;
    this.mMode = 0; --EnumMode
    
    this.draw = function( self )
        local switch = {}

        --認証範囲画像
        self.mRedMaru:draw( 600, 100 );
        self.mBlueMaru:draw( 200, 100 );

        --switchケース 
        switch[ Mode.MODE_WAIT ] = function ()
            self.mTitle_Auth:draw( 130, 400 );
        end
        switch[ Mode.MODE_NOW ] = function ()
           -- self.mTitle_Auth:draw( 130, 400 );

                self.mOK:draw( 130, 530 );
        end
        switch[ Mode.MODE_END ] = function ()
            self.mOK:draw( 130, 530 );
        end

        switch[ this.mMode ]();
        
    end

    this.handRang = function( self, checkPos, kinectPos )
        if ( mHandCheckCount > 0 ) then 
            mHandCheckCount = mHandCheckCount - 1;
            lb = checkPos.x - 200;
            rb = checkPos.x + 200;
            if ( ( kinectPos.x < rb ) and ( kinectPos.x > lb ) ) then
                tb = checkPos.y - 200;
                bb = checkPos.y + 200;
                if ( ( kinectPos.y < bb ) and ( kinectPos.y > tb ) ) then
                    mHandCheckCount = mHandCheckCount + 2;
                    return true;
                end
            end
        end 
    return false;
    end

    return this;

end