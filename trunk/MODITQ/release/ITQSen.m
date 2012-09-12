function [B,R] = ITQSen( X , W , n_iter)
%
% main function for ITQ Sensitivity which finds a rotation of the PCA embedded data
% Input:
%       X: n*d raw data, n is the number of images and d is dimension
%       W: d*c projection matrix, 
%       n_iter: max number of iterations, 50 is usually enough
% Output:
%       B: n*c binary matrix
%       R: the c*c rotation matrix found by ITQ
% Author:
%       IMS@SCUT
%       Yes, We can plot the J(R) to see how much iterations should we
%       take!
%       
%

% used to plot the correct time to stop
J = zeros( 1 , n_iter );
Q = diag( sqrt( var( X ) ) );
%

V = X * W;

% initialize with a orthogonal random rotation
bit = size(V,2);
R = randn(bit,bit);
[U11 S2 V2] = svd(R);
R = U11(:,1:bit);

% ITQ to find optimal rotation, R is orthogonal !! I am wrong!
for iter = 1 : n_iter
    Z = V * R;      
    UX = ones(size(Z,1),size(Z,2)).*-1;
    UX(Z>=0) = 1; 
    D = V' * UX;
    
    G = sqrtm( D * D' );
    R = inv( G ) * D;       % change inv(D) * G into inv(G)*D
    
    J(iter) = trace( 1/4 * (UX' * UX) ) - trace( UX' * X * W * R ) + trace( R' * W' * ( X' * X ) * W * R ) +  trace( R' * W' * Q * W * R );
end

% make B binary
B = UX;
B(B<0) = 0;

figure( 2 );
x = 1 : iter ;
plot( x , J , 'r.:' );
