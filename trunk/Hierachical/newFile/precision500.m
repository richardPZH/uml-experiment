function [] = precision500( precs , topRet )
% 
% This function is used to plot the precision 500 
% Input:
%      precs
%      topRet

numOfRet = ( 50:50:500 );
precision = zeros( 1 , 10 );

%adjust all topRet order
[ topRet index ] = sort( topRet );
precs = precs( index );

count = 0;
step = 1;
low = 1;
length = size( topRet , 2 );

Psum = 0;
while low <= length
    while ( low <= length ) && ( topRet( low ) <= 75 * step )
        Psum = Psum + precs( low );
        count = count + 1;
        low = low + 1;
    end
    
    precision( step ) = Psum / count;
    
    count = 0;
    step = step + 1;
    Psum = 0;
    
    if step == 11
        break;
    end
end



plot( numOfRet , precision , '-x' );
title( 'precision@500' );
axis( [ 0 500 0 1 ] );
grid on;
