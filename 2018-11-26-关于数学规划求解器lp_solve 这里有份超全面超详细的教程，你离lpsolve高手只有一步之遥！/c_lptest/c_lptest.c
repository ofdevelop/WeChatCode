// lpetest.cpp: 定义控制台应用程序的入口点。
//
/*
#include <stdio.h>
#include "lp_lib.h"

#pragma comment(lib, "lpsolve55.lib")
int main()
{
	lprec *lp;
	REAL row[1 + 3]; // must be 1 more then number of columns ! 
	lp = make_lp(0, 3);
	if (lp == NULL) {
		printf("Unable to create new LP model\n");
		return(1);
	}
	set_add_rowmode(lp, TRUE);//相当于设置开关，打开
	set_maxim(lp);//求解最大
	row[1] = 4;
	row[2] = 2;
	row[3] = 1;
	set_obj_fn(lp, row); // construct the obj: 4 x1 + 2 x2 + x3 

	row[1] = 2;
	row[2] = 1;
	row[3] = 0;
	add_constraint(lp, row, LE, 1);//约束1
	row[1] = 1;
	row[2] = 0;
	row[3] = 2;
	add_constraint(lp, row, LE, 2);//约束2
	row[1] = 1;
	row[2] = 1;
	row[3] = 1;
	add_constraint(lp, row, EQ, 1);//约束3
	set_bounds(lp, 1, 0.0, 1.0);//设置上下限
	set_bounds(lp, 2, 0.0, 1.0);//默认下限是0，其实可以用set_upbo
	set_bounds(lp, 3, 0.0, 2.0);
	set_add_rowmode(lp, FALSE);//开关关闭
	solve(lp);//求解
	print_solution(lp, 1);

	delete_lp(lp);
	return 0;
}

*/

#include "lp_lib.h"

#pragma comment(lib, "lpsolve55.lib")

int demo()
{
	lprec *lp;
	int Ncol, *colno = NULL, j, ret = 0;
	REAL *row = NULL;

	/* We will build the model row by row
	So we start with creating a model with 0 rows and 2 columns */
	Ncol = 2; /* there are two variables in the model */
	lp = make_lp(0, Ncol);
	if (lp == NULL)
		ret = 1; /* couldn't construct a new model... */

	if (ret == 0) {
		/* let us name our variables. Not required, but can be useful for debugging */
		set_col_name(lp, 1, "x");
		set_col_name(lp, 2, "y");

		/* create space large enough for one row */
		colno = (int *)malloc(Ncol * sizeof(*colno));
		row = (REAL *)malloc(Ncol * sizeof(*row));
		if ((colno == NULL) || (row == NULL))
			ret = 2;
	}

	if (ret == 0) {
		set_add_rowmode(lp, TRUE);  /* makes building the model faster if it is done rows by row */

									/* construct first row (120 x + 210 y <= 15000) */
		j = 0;

		colno[j] = 1; /* first column */
		row[j++] = 120;

		colno[j] = 2; /* second column */
		row[j++] = 210;

		/* add the row to lpsolve */
		if (!add_constraintex(lp, j, row, colno, LE, 15000))
			ret = 3;
	}

	if (ret == 0) {
		/* construct second row (110 x + 30 y <= 4000) */
		j = 0;

		colno[j] = 1; /* first column */
		row[j++] = 110;

		colno[j] = 2; /* second column */
		row[j++] = 30;

		/* add the row to lpsolve */
		if (!add_constraintex(lp, j, row, colno, LE, 4000))
			ret = 3;
	}

	if (ret == 0) {
		/* construct third row (x + y <= 75) */
		j = 0;

		colno[j] = 1; /* first column */
		row[j++] = 1;

		colno[j] = 2; /* second column */
		row[j++] = 1;

		/* add the row to lpsolve */
		if (!add_constraintex(lp, j, row, colno, LE, 75))
			ret = 3;
	}

	if (ret == 0) {
		set_add_rowmode(lp, FALSE); /* rowmode should be turned off again when done building the model */

									/* set the objective function (143 x + 60 y) */
		j = 0;

		colno[j] = 1; /* first column */
		row[j++] = 143;

		colno[j] = 2; /* second column */
		row[j++] = 60;

		/* set the objective in lpsolve */
		if (!set_obj_fnex(lp, j, row, colno))
			ret = 4;
	}

	if (ret == 0) {
		/* set the object direction to maximize */
		set_maxim(lp);

		/* just out of curioucity, now show the model in lp format on screen */
		/* this only works if this is a console application. If not, use write_lp and a filename */
		write_LP(lp, stdout);
		/* write_lp(lp, "model.lp"); */

		/* I only want to see important messages on screen while solving */
		set_verbose(lp, IMPORTANT);

		/* Now let lpsolve calculate a solution */
		ret = solve(lp);
		if (ret == OPTIMAL)
			ret = 0;
		else
			ret = 5;
	}

	if (ret == 0) {
		/* a solution is calculated, now lets get some results */

		/* objective value */
		printf("Objective value: %f\n", get_objective(lp));

		/* variable values */
		get_variables(lp, row);
		for (j = 0; j < Ncol; j++)
			printf("%s: %f\n", get_col_name(lp, j + 1), row[j]);

		/* we are done now */
	}

	/* free allocated memory */
	if (row != NULL)
		free(row);
	if (colno != NULL)
		free(colno);

	if (lp != NULL) {
		/* clean up such that all used memory by lpsolve is freed */
		delete_lp(lp);
	}

	return(ret);
}

int main()
{
	demo();
}
