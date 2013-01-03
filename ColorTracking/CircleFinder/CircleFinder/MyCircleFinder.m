function [ r , c , radius ] = MyCircleFinder ()


info = imaqhwinfo;

winfo = imaqhwinfo( 'winvideo' );

obj = videoinput( 'winvideo' , 1 , 'YUY2_320x240' );

set( obj , 'ReturnedColorSpace' , 'rgb' );

preview( obj );

hdl1 = figure( 2 );
hdl2 = figure( 3 );

frame = getsnapshot( obj );
figure( 2 );
imshow( frame );

rect = getrect;
window.r = floor( rect(2) + rect(4)/2 );
window.c = floor( rect(1) + rect(3)/2 );
window.radius = 55;

frame = getsnapshot( obj );
[ rM , cM , color ] = size( frame );

while( 1 )
    
    frame = getsnapshot( obj );
    hframe = rgb2gray( frame );
    hframe = hframe( : , : , 1 );
        
    rmin = window.r - window.radius;
    rmax = window.r + window.radius;
    cmin = window.c - window.radius;
    cmax = window.c + window.radius;
    
    if rmin < 1
        rmin = 1;
    end
    if rmax > rM
        rmax = rM;
    end
    if cmin < 1
        cmin = 1;
    end
    if cmax > cM
        cmax = cM;
    end
    
    searchArea = hframe( rmin:rmax , cmin:cmax , : );
    
    [ r , c , rad ] = circlefinder( searchArea , 30 , 35 );
    
    if isempty( r )
        window.r = 1;
        window.c = 1;
        window.radius = max( [rM cM] );
        continue;
    elseif rad(1) < 20 || rad(1) > 35
        window.r = 1;
        window.c = 1;
        window.radius = max( [rM cM] );
        continue;
    else
        %we do it manaually
        rr = floor( r(1) );
        cc = floor( c(1) );
    
        rr = rmin + rr;
        cc = cmin + cc;
        window.r = rr;
        window.c = cc;
        radius = floor( rad( 1 ) );
   
        sqTemp = ones( 9 , 9 , 'uint8' );
        sqTemp = sqTemp * 255;
        frame( rr - 4 : rr + 4 , cc - 4 : cc + 4 , 1 ) = frame( rr - 4 : rr + 4 , cc - 4 : cc + 4 , 1 ) + sqTemp;
    
        figure( hdl2 );
        imshow( frame );
        title('Find Circle');
    
        hold off;
    end
    
end

%     H = rgb2hsv( frame );          %get a hsv image
%     HImage = uint8( H(:,:,1) );           %get a H partical image
%     bw = edge( HImage , 'canny' );
%                                           %apply the gausi
%     figure( hdl1 );
%     imshow( bw );
%     title('Canny Edge');


%     sqTemp = zeros( 2 * radius , 2 * radius , 'uint8');
%     sqTemp( 1 : 4 , : ) = 255;
%     sqTemp( end-4+1:end , : ) = 255;
%     sqTemp( : , 1:4 ) = 255;
%     sqTemp( : , end-4+1:end ) = 255;
%     
%     frame( rr - radius : rr + radius - 1 , cc - radius : cc + radius - 1 , 1 ) = frame( rr - radius : rr + radius - 1 , cc - radius : cc + radius - 1 , 1 ) + sqTemp;
    

    