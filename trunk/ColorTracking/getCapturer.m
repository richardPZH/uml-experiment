function [ capturer ] = getCapturer ( inputObj , method )

% This function is used to capture pixels that use to trace
% We can use different method to capture pixels
% The red is predefined
% Author:
%       IMS@SCUT Once 
%       Heavy Carrier
%
% Retval:
%      a struct capturer | use more statistic way to handle ! ! !
%      .location, center point in the image, a 1x2 vector, row=location(1), col=location(2);
%      .data, the mx3 points capture in the rgb space, 
%
%

if nargin < 1
    method = 'default_red' ;
elseif nargin < 2
    method = 'default_red' ;
end

% This tries to behave like a factory. But it is to simple and stupid
% switch is slow, we can do better
switch method
    case 'default_red'
        % We use the predefined(capture) red points to work, we can get this in the RGB space
        % The capturer has the initial location, but in this case no
        % initial precisoin is given
        load 'red.mat'     % get data , a mx3 [ r g b ] vector
        
        capturer.location = [ 128 , 128 ];  %give this initial location is row=128,col=128
        capturer.data = data;
        
    case 'getpts'
        % prepare defalut paramaters
        windowSize = [ 14 14 ];       % the open window
                                      % each click will grabs a mxn pixel centrered at the pointer location
        %
        hdl = figure();
        frame = getsnapshot( inputObj );

        imshow( frame );

        m = floor( windowSize(1) / 2 );
        n = floor( windowSize(2) / 2 );
        
        title('Click To Select Points And Press Enter to End Selecting');
        % Need the switch X and Y, X is row, Y is col in the image matrix
        [X,Y] = getpts( hdl );
        Z = floor( X );
        X = floor( Y );
        Y = Z;
        clear Z; 
    
        %no subsample of the selected windows, we may subsample too!
        index = 1;
        r = zeros( 1 , (2*m+1) * (2*n+1) * length(X) );
        g = r;
        b = r;
        l = ( 2*m + 1 ) * ( 2*n + 1 ) - 1;

        for a = 1 : length( X )
            tmp = frame( ( X(a) - m ) :( X(a) + m ) , ( Y(a) - n ) : ( Y(a) + n ) , : );
            r( index : index + l ) = reshape( tmp( : , : , 1 ) , 1 , [] );
            g( index : index + l ) = reshape( tmp( : , : , 2 ) , 1 , [] );
            b( index : index + l ) = reshape( tmp( : , : , 3 ) , 1 , [] );
   
            index = index + l + 1;
        end

        %rgbHdl = figure( 3 );
        %plot3( r , g , b , 'o' , 'markerfacecolor' , 'b' );
        %axis( [ 0 255 0 255 0 255 ] );
        %grid on;
        %xlabel('red');
        %ylabel('green');
        %zlabel('blue');

        %start to find the ellipsoid in arbitrary orention
        data = [ r' , g' , b' ];
        centerPoint = [ floor( mean( X ) ) , floor( mean( Y ) ) ];
        
        capturer.location = centerPoint;
        capturer.data = data;
        
   case 'getrect'
        %we can use several getrect to capture more color
        %default region is 4
        nregion = 4;
        
        hdl = figure();
        frame = getsnapshot( inputObj );
        nframe = frame;
        imshow( frame );
        
        title([ 'Please Select ' num2str(nregion) ' Rectangle Regions' ]);
        
        centerPoint = [ 0 , 0 ];
        r = 0;
        g = 0;
        b = 0;
        for nr = 1 : nregion
            % rect is a four-element vector with the form [xmin ymin width height]
            rect = getrect( hdl );
        
            col = rect( 1 );
            row = rect( 2 );
            w = rect( 3 );
            h = rect( 4 );
            
            centerPoint = [ centerPoint ; row + h /2 , col + w / 2 ];
            
            for numR = row : row + h
                for numC = col : col + w 
                    tmp = frame( numR , numC , : );
                    r = [ r tmp(1) ];
                    g = [ g tmp(2) ];
                    b = [ b tmp(3) ];
                end
            end
            
            width = 2;
            mode = zeros( h+1 , w+1 , 'uint8' );
            mode( 1:width , : ) = 255;
            mode( end-width+1:end , : ) = 255;
            mode( : , 1:width ) = 255;
            mode( : , end-width+1:end ) = 255;
            nframe( row : row + h , col : col + w , 1 ) = nframe( row : row + h , col : col + w  , 1) + mode;
            nframe( row : row + h , col : col + w , 2 ) = nframe( row : row + h , col : col + w  , 2) + mode;
            nframe( row : row + h , col : col + w , 3 ) = nframe( row : row + h , col : col + w  , 3) + mode;
            
            imshow( nframe );
        end

        %start to find the ellipsoid in arbitrary orention
        r(1)=[];
        g(1)=[];
        b(1)=[];
        data = [ r' , g' , b' ];
        centerPoint( 1 , : ) = [];
        
        capturer.location = floor( mean( centerPoint ) );
        capturer.data = data;
        
    otherwise
        error('Error Using getCapturer'); %haha I got error instead of disp
                
end

title('Finished Selecting');
figure;
plot3( data(:,1) , data(:,2) , data(:,3) , 'o' , 'markerfacecolor' , 'r' )
title('RGB-Space, Original SamplePoints');
grid on;
axis( [0 255 0 255 0 255 ] );
xlabel('reg');
ylabel('green');
zlabel('blue');
