function [ C , R ] = compressCCAITQ( XX , label ,  bit )
%this funciton use CCA to compress original data first and then use the
%ITQ to find a good rotation R
%USE CCA to get Wk so that V = X * Wk ; 
% 
% Input:
%       X: n*d data matrix, n is number of images, d is dimension
%       Y: n*t class label response to X
%       bit: number of bits
% Output:      
%       C: n*bit binary code matrix
%       R: the founded rotation
%
% Details:
% label is the class lable of a row vector, for CIFAR, it is clean
% labels , just 1 2 4 5 - num of class
% bit is the number of bit to be used
% IMS@SCUT

% center the data, VERY IMPORTANT for ITQ to work
X = double( XX );
sampleMean = mean(X,1);
X = (X - repmat(sampleMean,size(X,1),1));

% Convert the label information into the Y¡Ê{0,1}(nxt)
t = max( label ) + 1;               % Well, I have already known that the CIFAR labels vary from 0 ~ MaxClassNum and they are all clean!
Y = zeros( size( label , 1 ) , t ); % Y is n x t
for i = 1 : size( label , 1 )       % Do we have a much faster way in Matlab use its property or cache?
    Y( i , label(i) + 1 ) = 1;
end

% Apply the CCA, need to prove a little bit later , we find the W == V
p = 0.0001;                         % follow the author in ITQ
A = ( X' * Y  ) / ( Y'*Y + p * eye( size( Y , 2 ) ) ) * Y' * X ;         %for matlab no using inv...
B = ( X' * X + p * eye( size( X , 2 ) ));
[ V D ] = eigs( A , B , bit );
for i = 1 : size( V , 2 )           % the eigenvectors is scaled by the eigenvalues
    V( : ,i ) = V( : , i ) / D( i , i );    
end

% now use the CCA found Wk == V to project original data
X = X * V;

% ITQ to find optimal rotation
% default is 50 iterations
% C is the output code
% R is the rotation found by ITQ
[C, R] = ITQ( X , 50 );


