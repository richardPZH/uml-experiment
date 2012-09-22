function [ ] = ShowCIFAR( X )
% This function will show the Picture of CIFAR
% How the CIFAR is organise is determined on the website
% Input:
%      X the CIFAR Dataset , n x 3072 , n is the number of the picture
%
% Author:
%       IMS@SCUT Once


%check the X dimension
[ n d ] = size( X );
    
% d should be 1024
d = d / 3;   

for i=1:n

    % XX is just a row vector [ x1 x2 ... xd ];
    XX = X( i , : );   

    Img = [ reshape( XX( 1:d ) , 32 , 32 ) reshape( XX( d+1:d*2 ) , 32 , 32 ) reshape( XX( 2*d+1:end ) , 32 , 32 ) ];
    Img = [ Img( : , 1:32 )' Img( : , 33:64 )' Img( : , 65:end )' ];
    Img = reshape( Img , 32 , 32 ,3 );

    % Using image() to show the image to human
    [ m , n , o ] = size( Img );
    figure( 'Units' , 'Pixels' , 'Position' , [100 100 m n ] );
    image( Img );
    axis image off;
    set( gca , 'Position' , [0 0 1 1 ]);

end





