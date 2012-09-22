function [ ] = ShowCIFAR( X )
% This function will show the Picture of CIFAR
% How the CIFAR is organise is determined on the website
% Input:
%      X the CIFAR Dataset , n x 3072 , n is the number of the picture
%
% Author:
%       IMS@SCUT Once
%       2012/09/22


%check the X dimension
[ n d ] = size( X );

% Try to plot the image on the same Figure in the l x l square 
l = ceil( sqrt( n ) ) ;   
a = 32 ;
b = 32 ;
%figure( 'Units' , 'Pixels' , 'Position' , [100 100 a b ] );
set( gca , 'Position' , [ 0 0 1 1 ] );
    
% d should be 1024
d = d / 3;   

for i=1:n

    % XX is just a row vector [ x1 x2 ... xd ];
    XX = X( i , : );   

    Img = [ reshape( XX( 1:d ) , 32 , 32 ) reshape( XX( d+1:d*2 ) , 32 , 32 ) reshape( XX( 2*d+1:end ) , 32 , 32 ) ];
    Img = [ Img( : , 1:32 )' Img( : , 33:64 )' Img( : , 65:end )' ];
    Img = reshape( Img , 32 , 32 ,3 );

    % Using image() to show the image to human
    subplot( l , l , i );
	image( Img );
    axis image off;
end





