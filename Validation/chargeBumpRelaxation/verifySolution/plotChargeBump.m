% Simulation settings for a domain ((-0.5,-0.5)--(0.5,0.5))
% with a centered charge bump
a = 0.05;
eps = 2;
K = 1;
timesToLoad=[0 2 4 6];

% Initial profile at t=0 through central axis (x/y)
pos  = -0.5:0.001:0.5;
init = exp(-pos.^2/(2*a*a))/(a*sqrt(2*pi));
figure(1);
hold on; box on;
plot(pos,init, '-r');
title('Initial charge profile');
xlabel('Position x/y');
ylabel('\rho_E');

% Compare spatial distribution
figure(2);
hold on; box on;
for i = 1:length(timesToLoad)
    % Load the file from the sample utility
    fname=['sets/' num2str(timesToLoad(i)) '/rhoE_lineX_rhoE.xy'];
    data = importdata(fname);
    % Plot the simulation data and analytical solution
    plot(data(:,1), data(:,2), 'o');
    plot(pos,init*exp(-K*timesToLoad(i)/eps), '-');
end
title('Charge bump relaxation');
xlabel('Position x/y');
ylabel('\rho_E');

% Compare transient of central (maximum) charge
listing=dir('sets'); % Need a 'sets' directory due to 'sample' utility
transient = [];
for i = 1:length(listing)
    if ~((strcmp(listing(i).name, '.') || (strcmp(listing(i).name, '..'))))
        fname=['sets/' listing(i).name '/somePoints_rhoE.xy'];    
        transient = [transient; str2num(listing(i).name) importdata(fname)];
    end
end
figure(3);
plot(transient(:,1), transient(:,5), 'o');
hold on; box on;
time=0:max(transient(:,1))/500:max(transient(:,1));
maxCharge=exp(-0/(2*a*a))/(a*sqrt(2*pi)); % at x=y=0
plot(time,maxCharge*exp(-K*time/eps), '-');
title('Transient relaxation of charge maximum');
xlabel('Time');
ylabel('\rho_{E,max}');