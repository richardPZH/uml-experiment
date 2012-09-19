function [] = IMStest( XX , labels ,  bit , method )
% 
% demo code for generating small code and evaluation
% input XX should be a n*d matrix, n is the number of images, d is dimension
% ''method'' is the method used to generate small code
% ''method'' can be 'ITQ', 'RR', 'LSH' and 'SKLSH' 
%

XX = double( XX );
X  = XX;

% parameters
%averageNumberNeighbors = 50;    % ground truth is 50 nearest neighbor
num_test = 1000;                % 1000 query test point, rest are database
bit = bit;                      % bits used


% split up into training and test set
[ndata, D] = size(X);
R = randperm(ndata);
Xtest = X(R(1:num_test),:);
XtestLabels = labels( R(1:num_test) );

R(1:num_test) = [];
Xtraining = X(R,:);
XtrainingLabels = labels( R );
num_training = size(Xtraining,1);
clear X;

% get the label true neighbor
WtrueTestTraining = zeros( num_test , num_training );
A = repmat( XtestLabels , 1 , size( XtrainingLabels , 1 ) );
B = repmat( XtrainingLabels' , size( Xtest , 1 ) , 1 );
A = A - B;
WtrueTestTraining( A == 0 ) = 1;


% generate training ans test split and the data matrix ; ALWAYS REMEMBER
% the X is the [Xtraining ; Xtest ];
X = [Xtraining; Xtest];
% center the data, VERY IMPORTANT
sampleMean = mean(X,1);
X = (X - repmat(sampleMean,size(X,1),1));

switch( method )
    case 'CCAITQ'   
    % Convert the label information into the Y（{0,1}(nxt)
    t = max( XtrainingLabels ) + 1;               % Well, I have already known that the CIFAR labels vary from 0 ~ MaxClassNum and they are all clean!
    Y = zeros( size( XtrainingLabels , 1 ) , t ); % Y is n x t
    for i = 1 : size( XtrainingLabels , 1 )       % Do we have a much faster way in Matlab use its property or cache?
        Y( i , XtrainingLabels(i) + 1 ) = 1;
    end

    % Apply the CCA, need to prove a little bit later 
    [Wx, r] = cca( X(1:num_training , : ) , Y , 0.0001 );

    r = r( 1:bit );
    r = r';
    Wx = Wx( : , 1:bit );              % for c bit code , get the leading Wx
    r = repmat( r , size( Wx , 1 ) , 1 );
    Wx = Wx .* r;                      % the Wx is scaled by its eigenvalue

    % now use the CCA found Wk to project original data
    X = X * Wx;

    % ITQ to find optimal rotation
    % default is 50 iterations
    % C is the output code
    % R is the rotation found by ITQ
    [C, R] = ITQ( X(1:num_training, :) , 50 );
    
    X = X*R;
    Y = zeros(size(X));
    Y(X>=0) = 1;
    Y = compactbit(Y>0);
    
    case 'CCAITQRR'
        % Convert the label information into the Y（{0,1}(nxt)
        t = max( XtrainingLabels ) + 1;               % Well, I have already known that the CIFAR labels vary from 0 ~ MaxClassNum and they are all clean!
        Y = zeros( size( XtrainingLabels , 1 ) , t ); % Y is n x t
        for i = 1 : size( XtrainingLabels , 1 )       % Do we have a much faster way in Matlab use its property or cache?
            Y( i , XtrainingLabels(i) + 1 ) = 1;
        end

    % Apply the CCA, need to prove a little bit later 
    [Wx, r] = cca( X(1:num_training , : ) , Y , 0.0001 );

    r = r( 1:bit );
    r = r';
    Wx = Wx( : , 1:bit );              % for c bit code , get the leading Wx
    r = repmat( r , size( Wx , 1 ) , 1 );
    Wx = Wx .* r;                      % the Wx is scaled by its eigenvalue



        % now use the CCA found Wk == V to project original data
        X = X * Wx;
    
        % RR
        R = randn(size(X,2),bit);
        [U S V] = svd(R);
        X = X*U(:,1:bit);
        Y = compactbit(X>0);
        
   % SKLSH
   % M. Raginsky, S. Lazebnik. Locality Sensitive Binary Codes from
   % Shift-Invariant Kernels. NIPS 2009.
    case 'SKLSH' 
        RFparam.gamma = 1;
        RFparam.D = D;
        RFparam.M = bit;
        RFparam = RF_train(RFparam);
        B1 = RF_compress(X(1:num_training,:), RFparam);
        B2 = RF_compress(X(num_training+1:end,:), RFparam);
        Y = [B1;B2];
    % Locality sensitive hashing (LSH)
     case 'LSH'
        X = X * randn(size(X,2),bit);
        Y = zeros(size(X));
        Y(X>=0)=1;
        Y = compactbit(Y);
        
    case 'OURSITQ'
        % Convert the label information into the Y（{0,1}(nxt)
        t = max( XtrainingLabels ) + 1;               % Well, I have already known that the CIFAR labels vary from 0 ~ MaxClassNum and they are all clean!  
        Y = zeros( size( XtrainingLabels , 1 ) , t ); % Y is n x t
        for i = 1 : size( XtrainingLabels , 1 )       % Do we have a much faster way in Matlab use its property or cache?
            Y( i , XtrainingLabels(i) + 1 ) = 1;
        end

    % Apply the CCA, need to prove a little bit later 
    [Wx, r] = cca( X(1:num_training , : ) , Y , 0.0001 );

    r = r( 1:bit );
    r = r';
    Wx = Wx( : , 1:bit );              % for c bit code , get the leading Wx
    r = repmat( r , size( Wx , 1 ) , 1 );
    Wx = Wx .* r;                      % the Wx is scaled by its eigenvalue

        % now use the CCA found Wk  to project original data      
        n_iter = 50;
        [B R] = ITQSen( X(1:num_training , : ) , Wx , n_iter );
        
        X = X * Wx * R ;
        Y = zeros(size(X));
        Y(X>=0) = 1;
        
        Y = compactbit(Y>0);
        
        
    case 'CCAPR'   
    % Convert the label information into the Y（{0,1}(nxt)
    t = max( XtrainingLabels ) + 1;               % Well, I have already known that the CIFAR labels vary from 0 ~ MaxClassNum and they are all clean!
    Y = zeros( size( XtrainingLabels , 1 ) , t ); % Y is n x t
    for i = 1 : size( XtrainingLabels , 1 )       % Do we have a much faster way in Matlab use its property or cache?
        Y( i , XtrainingLabels(i) + 1 ) = 1;
    end

    % Apply the CCA, need to prove a little bit later 
    [Wx, r] = cca( X(1:num_training , : ) , Y , 0.0001 );

    r = r( 1:bit );
    r = r';
    Wx = Wx( : , 1:bit );              % for c bit code , get the leading Wx
    r = repmat( r , size( Wx , 1 ) , 1 );
    Wx = Wx .* r;                      % the Wx is scaled by its eigenvalue
     

    % ITQ to find optimal rotation
    % default is 50 iterations
    % C is the output code
    % R is the rotation found by ITQ
    [C, R] = procrustean( X(1:num_training, :), Wx , XtrainingLabels , 500 );
    
    X = X * Wx * R;
    Y = zeros(size(X));
    Y(X>=0) = 1;
    Y = compactbit(Y>0);
        
end

% compute Hamming metric and compute recall precision
B1 = Y(1:size(Xtraining,1),:);
B2 = Y(size(Xtraining,1)+1:end,:);
Dhamm = hammingDist(B2, B1);
[recall, precision, rate] = recall_precision(WtrueTestTraining, Dhamm);

% plot the curve
switch(method)
    % ITQ method proposed in our CVPR11 paper
    case 'CCAITQ'
    plot(recall,precision,'-o');
    case 'CCAITQRR'
    plot(recall,precision,'-s');
    case 'SKLSH' 
    plot(recall,precision,'-d');
     case 'LSH'
    plot(recall,precision,'-<');
    case 'OURSITQ'
    plot(recall,precision,'-X');
    case 'CCAPR'
    plot(recall , precision,'-p');
end

xlabel('Recall');
ylabel('Precision');




