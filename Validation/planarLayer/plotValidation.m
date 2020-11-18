% Load data from interFoamEHD simulations
val_dc=importdata('dielectric-conductive/sets/2e-07/lineY_Ue_alpha1_magGradUe_p_rgh.xy');
val_cc=importdata('conductive-conductive/sets/10/lineY_Ue_alpha1_magGradUe_p_rgh.xy');
val_dd=importdata('dielectric-dielectric/sets/0.0005/lineY_Ue_alpha1_magGradUe_p_rgh.xy');

figure;
% Conductive-Conductive
y = val_cc(:,1);
Ue = val_cc(:,2);
alpha1 = val_cc(:,3);
EE = val_cc(:,4);

% Parameters for conductive-conductive
L=1;
eps1=2;
eps2=1;
sgm1=1;
sgm2=4;
R=sgm1/sgm2
Q=eps1/eps2

% Analytical solution conductive-conductive
phi1_ex = (-2.*y+R)/(1.+R)
phi2_ex = R*(-2.*y+1.)/(1.+R)

% Create the subplot
subplot(1,3,1);
plot(y,Ue,'o');
hold on;
plot([y(y<=0);y(y>0)],[phi1_ex(y<=0);phi2_ex(y>0)]);
xlabel('Position [m]');
ylabel('\phi [V]');
title('Conductive-Conductive');
legend('Simulation','Exact');

% Dielectric-Dielectric
y = val_dd(:,1);
Ue = val_dd(:,2);
alpha1 = val_dd(:,3);
EE = val_dd(:,4);

% Parameters for D-D
eps1       = 3e-11;
eps2       = 1e-11;
Q=eps1/eps2;

% Analytical solution for D-D
phi1_ex = (-2*y+Q)/(1+Q);
phi2_ex = (Q*(-2*y+1))/(1+Q);
E_ex=zeros(size(y));
deltap_ex = -(2*Q*(Q-1))/((1+Q)^2)

% Plot the solutions for D-D
subplot(1,3,2);
plot(y,Ue,'o');
hold on;
plot([y(y<=0);y(y>0)],[phi1_ex(y<=0);phi2_ex(y>0)]);
xlabel('Position [m]');
ylabel('\phi [V]');
title('Dielectric-Dielectric');
legend('Simulation','Exact');

% Dielectric-Conductive
y = val_dc(:,1);
Ue = val_dc(:,2);
alpha1 = val_dc(:,3);
EE = val_dc(:,4);

% Parameters for D-C
phi1_ex(y<0) = 1.;
phi2_ex(y>0) = 1.-2.*y(y>0);
E_ex=zeros(size(y))
deltap_ex = -2.
% E_ex[i1]=0.0
% E_ex[i2]=2.0

subplot(1,3,3);
plot(y,Ue,'o');
hold on;
plot([y(y<=0);y(y>0)],[phi1_ex(y<=0);phi2_ex(y>0)]);
xlabel('Position [m]');
ylabel('\phi [V]');
title('Dielectric-Conductive');
legend('Simulation','Exact');

% Now follows the multi-region validation of the Gauss's Law coupling
% between the two regions using a dielectric-dielectric in the fluid
% region, and a fully dielectric region as solid (can't be other than
% dielectric).
figure;
val_MR_S=importdata('dielectric-dielectric_MultiRegion/sets/Solid/0.2/lineY_Ue.xy');
val_MR_F=importdata('dielectric-dielectric_MultiRegion/sets/Fluid/0.2/lineY_Ue_alpha1_p_rgh.xy');

% Values of permittivity
eps1  = 2e-11; % Fluid1
eps2  = 4e-11; % Fluid2
epsMR = 1e-11; % Solid

% Analytical solution
eps_sum = 1/eps1 + 1/eps2 + 1/epsMR;
y = [val_MR_S(:,1); val_MR_F(:,1)];
Ue_Solid  = (y(y<0)+0.25)*(1/epsMR) / eps_sum;
Ue_Fluid1 = 0.25*(1/epsMR) / eps_sum + (y(y>0 & y<0.25))*(1/eps2) / eps_sum;
Ue_Fluid2 = 0.25*(1/epsMR) / eps_sum + 0.25*(1/eps2) / eps_sum +(y(y>0.25)-0.25)*(1/eps1) / eps_sum;

Ue_Total = 4*[0; Ue_Solid; Ue_Fluid1; Ue_Fluid2; 0.25];

Ue_Sim = [val_MR_S(:,2); val_MR_F(:,2)];

% Plotting
plot(y,Ue_Sim, 'o');
hold on;
plot([-0.25; y; 0.5],Ue_Total);
xlabel('Position [m]');
ylabel('\phi [V]');
title('Multiregion');
legend('Simulation','Exact');




% 
