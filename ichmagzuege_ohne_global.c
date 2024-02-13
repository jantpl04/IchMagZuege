#include <stdio.h>
#include <math.h>

//#define acceleration 1.2
//#define deceleration 0.9
#define ARRAY_SIZE 8
#define ACCELERATION 1.2 // Beschleunigung in m/s^2
#define DECELERATION -0.9 // Bremsen in m/s^2



//double distances[ARRAY_SIZE] = { 1.7, 0.4, 1.3, 0.2, 0.2, 0.8, 0.1 };
//double max_speeds[ARRAY_SIZE] = { 100, 80 , 50 , 45 , 50 , 75 , 50 };

//Hiiiiii

double calculate_time(double[], double[]);
void adjust_speed_limits(double*, double*);

int main()
{
	double distances[ARRAY_SIZE] = { 1.7, 0.2, 0.2, 0.3, 0.2, 0.2, 0.8, 0.1 };
	double max_speeds[ARRAY_SIZE] = { 100, 80 , 30 , 50 , 45 , 50 , 75 , 50 };

	for (int i = 0; i < ARRAY_SIZE; i++) {
		max_speeds[i] /= 3.6;
		distances[i] *= 1000;
	}

	adjust_speed_limits(distances, max_speeds);
	printf("%lf", calculate_time(distances, max_speeds));
}

double calculate_time(double distances[], double max_speeds[])
{
	double time = 0, current_time, current_speed = max_speeds[0], current_distance, acceleration_time, next_speed = 0;
	char x = 0;
	for (int i = 0; i < ARRAY_SIZE; i++) {
		current_time = 0;
		current_distance = distances[i];

		printf("Streckenabschnitt %d:\n", i + 1);
		//Muss Beschleunigt werden?
		if (current_speed < max_speeds[i]) {
			acceleration_time = (max_speeds[i] - current_speed) / ACCELERATION;
			current_distance -= 0.5 * ACCELERATION * acceleration_time * acceleration_time + current_speed * acceleration_time;
			printf("Beschleunige von %.2lf auf %.2lf. Vom Streckenabschnitt sind noch %.2lf m uebrig.\n", current_speed * 3.6, max_speeds[i] * 3.6, current_distance);
			current_time += acceleration_time;
			current_speed = max_speeds[i];
		}
		//Muss am Ende gebremst werden?
		if (current_speed > max_speeds[i + 1] && i < ARRAY_SIZE - 1) {
			acceleration_time = (max_speeds[i + 1] - current_speed) / DECELERATION;
			current_distance -= 0.5 * DECELERATION * acceleration_time * acceleration_time + current_speed * acceleration_time;
			printf("Bremse am Ende von %.2lf auf %.2lf. Vom Streckenabschnitt werden %.2lf m ohne Beschleunigung gefahren\n", current_speed * 3.6, max_speeds[i + 1] * 3.6, current_distance);
			current_time += acceleration_time;
			next_speed = max_speeds[i + 1];
			x = 1;
		}
		//Fahren der übrig gebliebenen Strecke
		current_time += current_distance / current_speed;
		if (x) {
			current_speed = next_speed;
		}
		x = 0;
		printf("Befahren von Abschnitt %d hat %.2lf s gedauert\n\n\n", i + 1, current_time);
		time += current_time;
	}
	return time;
}

void adjust_speed_limits(double* distances, double* max_speeds)
{
	double braking_time, braking_distance;
	for (int i = ARRAY_SIZE - 1; i > 0; i--) {
		//Nur nötig, wenn gebremst werden muss
		if (max_speeds[i] < max_speeds[i - 1]) {
			//testen ob die vorherige Strecke ausreicht um zu bremsen
			braking_time = (max_speeds[i] - max_speeds[i - 1]) / DECELERATION;
			braking_distance = 0.5 * DECELERATION * braking_time * braking_time + max_speeds[i - 1] * braking_time;
			if (braking_distance > distances[i - 1]) {
				//Falls Strecke nicht ausreicht, wird das speed_limit angepasst
				max_speeds[i - 1] = sqrt(max_speeds[i] * max_speeds[i] - 2 * DECELERATION * distances[i]);
			}
		}
	}
}