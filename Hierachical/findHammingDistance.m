function [ HCWDD ] = findHammingDistance( X , D )
% This function will return the hamming distance code of D in X
% 
% Input:
%     X is the binary code, 1 x n , n is the number of binary bit
%          e.g.  X = [1 0 0 1 1]
%     D is the hamming distance, e.g. 1 2 3 4...
%          D <= size( X , 2 )  is required
% 
% Output:
%     HCWDD == Hamming Codes With Distance D
%          sequence of code that hammingDistance( X , code )==D
%
% Authors:
%     IMS@SCUT Once
%     2012/09/25
%
% Warning This function is under develop!!!

if D > size( X , 2 )
	D = size( X , 2 );
end

% Matlab nchoosek calculate the C( N , K )
codeLength = size( X , 2 );
totalNum = nchoosek( codeLength , D );

HCWDD = zeros( totalNum , codeLength ); 



