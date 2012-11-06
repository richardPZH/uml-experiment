function [ ] = Level3Search( imageGist , imageVector , imageLabel , trGist , trLabels , trVector , index1 , index2 , E3 )
%
% This will search in the third level
%
%
%
% Author: 
%      IMS@SCUT Once 2012/11/05

% the hamming distance in level 2, which we need to perdefine

%Some evil global variables
global numOfRet;
global numOfHit;
global recall;
global precision;

L3Dis = 5;

anoymousEntrance = E3{ index1 };

% Get Level 3 info
W2=  anoymousEntrance{ index2 , 1 }{ 1 };
R2 = anoymousEntrance{ index2 , 1 }{ 2 };
cP2 = anoymousEntrance{ index2 , 1 }{ 3 };

L3Code = anoymousEntrance{ index2 , 2 };
L3Label = anoymousEntrance{ index2 , 3};

% This imageGist's binary code in Level3
code = ( imageGist - cP2  ) * W2 * R2;
code( code >= 0 ) = 1;
code( code <0   ) = 0;

hmd = CalHammingDist( code , L3Code , 'vec' );

for k = 0 : L3Dis
   %level 2 distance from 0 to L2Dis
   vec = find( hmd == k );
   
   for a = 1 : size( vec , 1 )
       
       
   end
   
   %level 2 end
end








