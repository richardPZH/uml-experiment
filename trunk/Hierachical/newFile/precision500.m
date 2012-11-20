function [] = precision500( recall , precision )
% 
% This function is used to plot the precision@500 
% Input:
%      recall 
%      precision
%
%

% Because the numOf samples is 5000
numOfSam = 5000;

numOfRet = ( numOfSam * recall ) ./ precision ;

plot( numOfRet , precision , '-x' );
grid on;
