% Title:        Consat-1 Payload Simulation Script
% Description:  Script for setting the workspace variables to be used in the
%               "Consat1Payload" simulation. 
% Author:       Austin Hubbell

% Status:       Work in progress.


%Simulation time
time = [0:0.01:60]'; %simulate 1 minute

%PWM wave attributes
pwmAmplitude = [0:0.5:9]';
pwmDutyCycle = [0:0.1:100]'; %simulation takes duty cycle as a percentage

%Gain for flyback DC-DC converter
flybackGain = [1:0.5:100]'; %must be modified for 300-500V (dependent on PWM amplitude)


%CHOOSE ONE OF THE FOLLOWING TO SIMULATE:


%Simulate for no detected peaks
peaks = zeros(length(time), 1);
numPeaks = [time peaks];

%Simulate for one detected peak
peaks = zeros(length(time), 1);
peaks(round(rand*length(peaks))) = 1; %position of peak in array is randomized
numPeaks = [time peaks];

%Simulate for one detected peak in multiple measurements
peaks = zeros(length(time), 1);
for i = 1:1:length(peaks)
    peaks(ceil(rand*length(peaks))) = 1;
end
numPeaks = [time peaks];

%Simulate for multiple detected peaks in one measurement (one element > 1)
peaks = zeros(length(time), 1);
peaks(round(rand*length(peaks))) = ceil(rand*1e2)+1; %position of peak in array is randomized, as well as the number of peaks
numPeaks = [time peaks];

%Simulate for multiple detected peaks in multiple measurements
peaks = zeros(length(time), 1);
for i = 1:1:length(peaks)
    peaks(ceil(rand*length(peaks))) = ceil(rand*1e2);
end
numPeaks = [time peaks];


