#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <Windows.h>

struct Point
{
	int x;
	int y;
	int z;
};
typedef struct Point point;

point* readPointsFromFile(char* path, point* points, int* readedPoints)
{
	int points_count;
	FILE* db_file;
	fopen_s(&db_file, path, "r");
	fscanf_s(db_file, "%d", &points_count);
	*readedPoints = points_count;
	char tmp_char;
	fscanf_s(db_file, "%c", &tmp_char, 1);
	points = (point*)malloc(points_count * sizeof(point));
	for (int i = 0; i < points_count; ++i)
	{
		fscanf_s(db_file, "%d", &(points[i].x));
		fscanf_s(db_file, "%d", &(points[i].y));
		fscanf_s(db_file, "%d", &(points[i].z));
		fscanf_s(db_file, "%c", &tmp_char, 1);
	}
	fclose(db_file);
	return points;
}
void outputPoint(point* point)
{
	printf("Point : %d %d %d\n", point->x, point->y, point->z);
}
void outputPoints(point* points, int points_count)
{
	for (int i = 0; i < points_count; ++i)
		outputPoint(&(points[i]));
}
void outputPointsToBynaryFile(point* points, int points_count, char* fileName)
{
	FILE* output;
	fopen_s(&output, fileName, "wb");
	fwrite(&points_count, 1, sizeof(int), output);
	fwrite(points, points_count, sizeof(point), output);
	fclose(output);
}
point* readPointsFromBinaryFile(point* points, int* points_count, char* fileName)
{
	FILE* input;
	errno_t err = fopen_s(&input, fileName, "rb");
	if (!err)
	{
		int count = 0;
		fread(&count, sizeof(int), 1, input);
		*points_count = count;
		printf("%d points in binary file\n", count);
		points = (point*)malloc(count * sizeof(point));
		fread(points, count, sizeof(point), input);
		fclose(input);
	}
	return points;
}
int main()
{
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	point* points = 0;
	int pointsCount = 0;
	int choise = 0;
	printf("input 0/1 (read/write from binary file)\n");
	scanf_s("%d", &choise);
	if (choise == 1)
	{
		points = readPointsFromFile("db.txt", points, &pointsCount);
		int massSize = 0;
		for (int i = 0; i < pointsCount; ++i)
			massSize += sizeof(points[i]);
		printf("readed %d points %d bytes memory used\n", pointsCount, massSize);
		outputPoints(points, pointsCount);
		outputPointsToBynaryFile(points, pointsCount, "db.byn");
	}
	else if (choise == 0)
	{
		points = readPointsFromBinaryFile(points, &pointsCount, "db.byn");
		outputPoints(points, pointsCount);
	}
	free(points);
	return 0;
}