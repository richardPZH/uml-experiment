function [recall, precision] = testPCALabel(X, labels, bit, method)
%
% Use the PCA to projected the data
% But Use the Label Information as the ground truth
%
% input:
% X is the n x d raw matrix
% labels is the label related to X
% bit the length of hashcode
% method choose the method


% parameters
averageNumberNeighbors = 50;    % ground truth is 50 nearest neighbor
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


% generate training ans test split and the data matrix
XX = [Xtraining; Xtest];
% center the data, VERY IMPORTANT
sampleMean = mean(XX,1);
XX = (XX - repmat(sampleMean,size(XX,1),1));


%several state of art methods
switch(method)
    
    % ITQ method proposed in our CVPR11 paper
    case 'ITQ'
        % PCA
        [pc, l] = eigs(cov(XX(1:num_training,:)),bit);
        XX = XX * pc;
        % ITQ
        [Y, R] = ITQ(XX(1:num_training,:),50);
        XX = XX*R;
        Y = zeros(size(XX));
        Y(XX>=0) = 1;
        Y = compactbit(Y>0);
    % RR method proposed in our CVPR11 paper
    case 'RR'
        % PCA
        [pc, l] = eigs(cov(XX(1:num_training,:)), bit);
        XX = XX * pc;
        % RR
        R = randn(size(XX,2),bit);
        [U S V] = svd(R);
        XX = XX*U(:,1:bit);
        Y = compactbit(XX>0);
   % SKLSH
   % M. Raginsky, S. Lazebnik. Locality Sensitive Binary Codes from
   % Shift-Invariant Kernels. NIPS 2009.
    case 'SKLSH' 
        RFparam.gamma = 1;
        RFparam.D = D;
        RFparam.M = bit;
        RFparam = RF_train(RFparam);
        B1 = RF_compress(XX(1:num_training,:), RFparam);
        B2 = RF_compress(XX(num_training+1:end,:), RFparam);
        Y = [B1;B2];
    % Locality sensitive hashing (LSH)
     case 'LSH'
        XX = XX * randn(size(XX,2),bit);
        Y = zeros(size(XX));
        Y(XX>=0)=1;
        Y = compactbit(Y);
        
    % our own novel 1 first itq, than find a good s to improve the
    % sensitivity
    case 'ITQS'
        % PCA
        [pc, l] = eigs(cov(XX(1:num_training,:)),bit);
        
        %Our kind of ITQ
        n_iter = 50;
        [B R S] = OURSITQ( XX( 1:num_training , : ) , pc , XtrainingLabels , n_iter );
        
        XX = XX * pc * R * S;
        Y = zeros(size(XX));
        Y(XX>=0) = 1;
        
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
    case 'ITQ'
    plot(recall,precision,'-o');
    case 'RR'
    plot(recall,precision,'-s');
    case 'SKLSH' 
    plot(recall,precision,'-d');
     case 'LSH'
    plot(recall,precision,'-<');
    case 'ITQS'
    plot(recall,precision,'-X'); 
end

xlabel('Recall');
ylabel('Precision');





