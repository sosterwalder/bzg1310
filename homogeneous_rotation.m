pkg load all
%%% Homogeneous rotation


%% Translation

% Identity matrix
matIdentity = [
    1, 0;
    0, 1
]

% Affine space transformation
transAffineSpace = [
    -1;
    -2
]

% Projective space matrix
matTranslationProjective = [
    matIdentity, transAffineSpace;
    zeros(1, 2), 1
]


%% Rotation

phi = pi / 3

% Linear space
matRotation = [
    cos(phi),   -sin(phi);
    sin(phi),    cos(phi)
]

% Projective space
matRotationProjective = [
    matRotation,    zeros(2, 1);
    zeros(1, 2),    1
]


%% Define elements
matElementZeroX = ones(3, 1) * [3:6]
matElementZeroY = [3:5]' * ones(1, 4)

% Homogenous coordinates
matElementZeroHomogenous = [
    reshape(matElementZeroX, 1, 12);
    reshape(matElementZeroY, 1, 12);
    ones(1, 12)
]

% Projective transform
matElementZeroProjective = matTranslationProjective * matRotationProjective * inv(matTranslationProjective) * matElementZeroHomogenous

% Affine coordinates for X and Y
matElementOneX = reshape(matElementZeroProjective(1, :), 3, 4)
matElementOneY = reshape(matElementZeroProjective(2, :), 3, 4)


%% Rotation arc
theta = atan2(matElementZeroY(1, 1) - transAffineSpace(2), matElementZeroX(1, 1) - transAffineSpace(1))
angle = linspace(theta, theta + phi);
radius = sqrt((matElementZeroY(1, 1) - transAffineSpace(2))^2 + (matElementZeroX(1, 1) - transAffineSpace(1))^2)

%% Graphics

% Axes
plot(transAffineSpace(1), transAffineSpace(2), 'ko', [-5, 6], [0, 0], 'k', [0, 0], [-2, 9], 'k')
hold on

% Element zero
plot(matElementZeroX, matElementZeroY, 'ro', matElementZeroX, matElementZeroY, 'r', matElementZeroX', matElementZeroY', 'r')
% Element one
plot(matElementOneX, matElementOneY, 'bo', matElementOneX, matElementOneY, 'b', matElementOneX', matElementOneY', 'b')

% Transformation line
plot(radius * cos(angle) + transAffineSpace(1), radius * sin(angle) + transAffineSpace(2), 'g')

% Axis
axis('square', [-8, 8, -4, 12])

hold off
