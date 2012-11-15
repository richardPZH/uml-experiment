function [ ] = Level3Search( imageGist , imageVector , imageLabel , trGist , trLabels , trVector , index2 , E3 )
%
% This will search in the third level
%
%
%
% Author: 
%      IMS@SCUT Once 2012/11/05

% the hamming distance in level 2, which we need to perdefine

%Some evil global variables
global numOfRet
global numOfHit
global numOfSam
global recall
global precision

L3Dis = 5;


% Get Level 3 info
W2=  E3{ index2 , 1 }{ 1 };
R2 = E3{ index2 , 1 }{ 2 };
cP2 = E3{ index2 , 1 }{ 3 };

L3Code = E3{ index2 , 2 };
L3Label = E3{ index2 , 3};

% This imageGist's binary code in Level3
code = ( imageGist - cP2  ) * W2 * R2;
code( code >= 0 ) = 1;
code( code <0   ) = 0;

hmd = CalHammingDist( code , L3Code , 'vec' );

for k = 0 : L3Dis
   %level 2 distance from 0 to L2Dis
   vec = find( hmd == k );
   
   for a = 1 : size( vec , 1 )
       
       retCell = L3Label{ vec(a) };
       
       numOfRet = numOfRet + size( retCell , 1 );
       numOfHit = numOfHit + sum( trLabels( retCell ) == imageLabel ); %because the retCell stores the label information 
       
   end
   
   recall =[ recall , numOfHit / numOfSam ];
   precision =[ precision , numOfHit / numOfRet ];
   
   %level 2 end
end








