function [ ] = Level2Search( imageGist , imageVector , imageLabel , trGist , trLabels , trVector , E2 , E3 )
%
% This function will search into the second level 
%
%
% 
% IMS@SCUT Once 2012/11/05

% the hamming distance in level 2, which we need to perdefine
L2Dis = 3;

% Get Level 2 info
W1=  E2{ 1 , 1 }{ 1 };
R1 = E2{ 1 , 1 }{ 2 };
cP1 = E2{ 1 , 1 }{ 3 };

L2Code = E2{ 1 , 2 };

% This imageGist's binary code in Level2
code = ( imageGist - cP1  ) * W1 * R1;
code( code >= 0 ) = 1;
code( code <0   ) = 0;

hmd = CalHammingDist( code , L2Code , 'vec' );

for k = 0 : L2Dis
   %level 2 distance from 0 to L2Dis
   vec = find( hmd == k );
   
   for a = 1 : size( vec , 1 )
       
        Level3Search( imageGist , imageVector , imageLabel , trGist , trLabels , trVector , vec( a ) , E3 );
       
   end
   
   %level 2 end
end


