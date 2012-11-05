function [ recall precision ] = Level2Search( recall , precision , imageGist , imageVector , imageLabel , trGist , trLabels , trVector , index , E2 , E3 )
%
% This function will search into the second level 
%
%
% 
% IMS@SCUT Once 2012/11/05

% Get Level 2 info
W1=  E2{ index , 1 }{ 1 };
R1 = E2{ index , 1 }{ 2 };
cP1 = E2{ index , 1 }{ 3 };

L2Code = E2{ index , 2 };

% This imageGist's binary code in Level2
code = ( imageGist - cP1  ) * W1 * R1;
code( code >= 0 ) = 1;
code( code <0   ) = 0;


