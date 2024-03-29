function [] = testall( X , bit )

%Let them have the same parameters
% parameters
averageNumberNeighbors = 50;    % ground truth is 50 nearest neighbor
num_test = 1000;                % 1000 query test point, rest are database
bit = bit;                      % bits used

% split up into training and test set
[ndata, D] = size(X);
R = randperm(ndata);
Xtest = X(R(1:num_test),:);
R(1:num_test) = [];
Xtraining = X(R,:);
num_training = size(Xtraining,1);
clear X;

% define ground-truth neighbors (this is only used for the evaluation):
R = randperm(num_training);
DtrueTraining = distMat(Xtraining(R(1:100),:),Xtraining); % sample 100 points to find a threshold
Dball = sort(DtrueTraining,2);
clear DtrueTraining;
Dball = mean(Dball(:,averageNumberNeighbors));
% scale data so that the target distance is 1
Xtraining = Xtraining / Dball;
Xtest = Xtest / Dball;
Dball = 1;
% threshold to define ground truth
DtrueTestTraining = distMat(Xtest,Xtraining);
WtrueTestTraining = DtrueTestTraining < Dball;
clear DtrueTestTraining


% generate training ans test split and the data matrix
XX = [Xtraining; Xtest];
% center the data, VERY IMPORTANT
sampleMean = mean(XX,1);
XX = (XX - repmat(sampleMean,size(XX,1),1));

testSameParameters( XX , Xtraining , WtrueTestTraining , D , num_training , bit , 'ITQ' );

grid on;
hold on;

testSameParameters( XX , Xtraining , WtrueTestTraining , D , num_training , bit , 'RR' );

testSameParameters( XX , Xtraining , WtrueTestTraining , D , num_training , bit , 'SKLSH' );

testSameParameters( XX , Xtraining , WtrueTestTraining , D , num_training , bit , 'LSH' );

testSameParameters( XX , Xtraining , WtrueTestTraining , D , num_training , bit , 'ITQS' );
