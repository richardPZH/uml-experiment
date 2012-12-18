function [ tk ] = Recover( tk , obj , alphaArea , subSample )

% 3.3 Recovery
% Occasionally the tracker will lose tis object b allowying it outside the
% tracking window or occlusion
%
% We define an object as "lost" when the size of its support falls below an
% aggregate image area threshold alphaArea
%
% Author:
%       IMS@SCUT Once

trackingWindowRadius2 = 64^2;

if nargin < 4
    subSample = [ 9 , 9 ];
end

subSampleNum = size( subSample , 1 ) * size( subSample , 2 );
walker = 1;

supportArea = 0;
preLocation = tk.Location;


frame = getsnapshot( obj );
[ row , col , color ] = size( frame );

X = zeros( ceil( row * col / ( subSample(1) * subSample(2)  ) )  , 1 );
Y = zeros( ceil( row * col / ( subSample(1) * subSample(2)  ) )  , 1 );

while( supportArea < alphaArea )
    index = 1;
    frame = getsnapshot( obj );
    figure( 3 );
    imshow( frame );
    
    if walker > subSampleNum
        walker = 1;
    end
    
    rcWalker = ceil ( walker / size( subSample , 2 ) );
    ccWalker = floor( mod( walker , size( subSample , 1 ) ) ) + 1;
    walker = walker + 1;
    
    for rc = rcWalker : subSample( 1 ) : row
        for cc = ccWalker : subSample( 2 ) : col
            
            tmp = frame( rc , cc , : );
            x = [ tmp(1); tmp(2) ; tmp(3) ];
                
            if isTargetColor( tk , x )
                X(index) = rc;
                Y(index) = cc;
                index = index + 1;
                frame( rc , cc , 1 ) = 255;
                frame( rc , cc , 2 ) = 255;
                frame( rc , cc , 2 ) = 255;
            end
            
        end
    end
    
    if index > 1
        index = index - 1;
    end
    
    data = [ X(1:index) , Y(1:index) ];
    lowIndex = 1 ;
    mak = 0 ;
    for t = 1 : index
        p = data( t , : );
        
        st = data - repmat( p , size( data , 1 ) , 1 );
        st = st.^2;
        st = sum( st , 2 );
        
        win = data( st(:) < trackingWindowRadius2 , : );
        win = win - repmat( preLocation , size( win , 1 ) , 1 );
        win = sum( win.^2 , 2 );
        area = sum( 1 ./ ( 1 + win ) );
                        
        if area > mak
            mak = area;
            lowIndex = t;
            
        end
    end
    
    supportArea = 3.14 * ( findRadius( data( lowIndex , :) , data(:,1) , data(:,2) , 'hyper' ))^2;
    
end

tk.Location = [ X(lowIndex) , Y(lowIndex) ];
tk.speed = [ 0 , 0 ];
