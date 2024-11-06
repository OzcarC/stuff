#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define MAX_LENGTH 1812
#define MAX_ENTRIES 3143
#define MAX_LINE_LENGTH 256
#define OPERATION_DELIM ":"

typedef struct {
    char county[MAX_LENGTH];
    char state[3];

    float education_bachelors;
    float education_highschool;

    float ethnicities_indian_alaska;
    float ethnicities_asian;
    float ethnicities_black;
    float ethnicities_hispanic;
    float ethnicities_pacific;
    float ethnicities_two_or_more;
    float ethnicities_white;
    float ethnicities_white_not_hispanic;

    int median_household_income;
    int per_capita_income;
    float persons_below_poverty;

    int population_2014;
} Demographic;

enum ColumnIndices {
    COL_COUNTY = 0,
    COL_STATE = 1,
    COL_EDU_BACHELORS = 5,    
    COL_EDU_HIGHSCHOOL = 6,   
    COL_ETH_INDIAN = 11,      
    COL_ETH_ASIAN = 12,       
    COL_ETH_BLACK = 13,       
    COL_ETH_HISPANIC = 14,    
    COL_ETH_PACIFIC = 15,     
    COL_ETH_TWO_RACE = 16,    
    COL_ETH_WHITE = 17,      
    COL_ETH_WHITE_NOT_HISPANIC = 18, 
    COL_INCOME_HOUSEHOLD = 25, 
    COL_INCOME_CAPITA = 26,    
    COL_INCOME_POVERTY = 27,   
    COL_POP_2014 = 37       
};

void remove_quotes(char *str) {
    char *src = str, *dst = str;
    while (*src) {
        if (*src != '"') {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}

FILE *open_file(const char *name) {
    FILE *file = fopen(name, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file %s\n", name);
        exit(1);
    }
    return file;
}

int load_demo_data(FILE *file, Demographic *data, int max_entries) {
char line[MAX_LENGTH];
int line_number = 0;
int counter = 0;
int empty_field_count = 0;
int incomplete_record_count = 0;


    while (fgets(line, sizeof(line), file) != NULL && counter < max_entries) {
        line_number++;
        line[strcspn(line, "\n")] = '\0';

        char *token = strtok(line, ",");
        int field_index = 0;
Demographic temp = {
    .county = "",
    .state = "",
    .education_bachelors = 0.0f,
    .education_highschool = 0.0f,
    .ethnicities_indian_alaska = 0.0f,
    .ethnicities_asian = 0.0f,
    .ethnicities_black = 0.0f,
    .ethnicities_hispanic = 0.0f,
    .ethnicities_pacific = 0.0f,
    .ethnicities_two_or_more = 0.0f,
    .ethnicities_white = 0.0f,
    .ethnicities_white_not_hispanic = 0.0f,
    .median_household_income = 0,
    .per_capita_income = 0,
    .persons_below_poverty = 0.0f,
    .population_2014 = 0
};

        while (token != NULL) {
            remove_quotes(token);

            switch (field_index) {
                case COL_COUNTY:
                    strncpy(temp.county, token, MAX_LENGTH - 1);
                    break;
                case COL_STATE:
                    strncpy(temp.state, token, 2);
                    temp.state[2] = '\0';
                    break;
                case COL_EDU_BACHELORS:
                    temp.education_bachelors = strtof(token, NULL);
                    break;
                case COL_EDU_HIGHSCHOOL:
                    temp.education_highschool = strtof(token, NULL);
                    break;
                case COL_ETH_INDIAN:
                    temp.ethnicities_indian_alaska = strtof(token, NULL);
                    break;
                case COL_ETH_ASIAN:
                    temp.ethnicities_asian = strtof(token, NULL);
                    break;
                case COL_ETH_BLACK:
                    temp.ethnicities_black = strtof(token, NULL);
                    break;
                case COL_ETH_HISPANIC:
                    temp.ethnicities_hispanic = strtof(token, NULL);
                    break;
                case COL_ETH_PACIFIC:
                    temp.ethnicities_pacific = strtof(token, NULL);
                    break;
                case COL_ETH_TWO_RACE:
                    temp.ethnicities_two_or_more = strtof(token, NULL);
                    break;
                case COL_ETH_WHITE:
                    temp.ethnicities_white = strtof(token, NULL);
                    break;
                case COL_ETH_WHITE_NOT_HISPANIC:
                    temp.ethnicities_white_not_hispanic = strtof(token, NULL);
                    break;
                case COL_INCOME_HOUSEHOLD:
                    temp.median_household_income = atoi(token);
                    break;
                case COL_INCOME_CAPITA:
                    temp.per_capita_income = atoi(token);
                    break;
                case COL_INCOME_POVERTY:
                    temp.persons_below_poverty = strtof(token, NULL);
                    break;
                case COL_POP_2014:
                    temp.population_2014 = atoi(token);
                    break;
            }

            token = strtok(NULL, ",");
            field_index++;
        }

        
        if (field_index == 52) {
            data[counter++] = temp;
        } else {
            incomplete_record_count++;
        }
    }

if (empty_field_count > 0 || incomplete_record_count > 0) {
fprintf(stderr, "\nData Loading Summary:\n");
        if (empty_field_count > 0) {
            fprintf(stderr, "- Found %d empty fields\n", empty_field_count);
        }
        if (incomplete_record_count > 0) {
            fprintf(stderr, "- Found %d incomplete records\n", incomplete_record_count);
        }
        fprintf(stderr, "Successfully loaded %d complete records\n", counter);
}

    return counter;
}

void print_demo_data(const Demographic *data, int num_records) {
    printf("\nDemographic Data (%d records):\n", num_records);
    for (int i = 0; i < num_records; i++) {
        printf("\nRecord %d:\n", i + 1);
        printf("County: %s, State: %s\n", data[i].county, data[i].state);
        printf("Education - Bachelor's: %.1f%%, High School: %.1f%%\n",
               data[i].education_bachelors, data[i].education_highschool);
        printf("Ethnicities:\n");
        printf("  Native American: %.1f%%\n", data[i].ethnicities_indian_alaska);
        printf("  Asian: %.1f%%\n", data[i].ethnicities_asian);
        printf("  Black: %.1f%%\n", data[i].ethnicities_black);
        printf("  Hispanic/Latino: %.1f%%\n", data[i].ethnicities_hispanic);
        printf("  Pacific Islander: %.1f%%\n", data[i].ethnicities_pacific);
        printf("  Two or More Races: %.1f%%\n", data[i].ethnicities_two_or_more);
        printf("  White: %.1f%%\n", data[i].ethnicities_white);
        printf("  White (not Hispanic): %.1f%%\n", data[i].ethnicities_white_not_hispanic);
        printf("Income:\n");
        printf("  Median Household: $%d\n", data[i].median_household_income);
        printf("  Per Capita: $%d\n", data[i].per_capita_income);
        printf("  Below Poverty: %.1f%%\n", data[i].persons_below_poverty);
        printf("2014 Population: %d\n", data[i].population_2014);
    }
}


float get_field_value(const Demographic* entry, const char* field) {
if (strcmp(field, "Education.Bachelor's Degree or Higher") == 0) return entry->education_bachelors;
    if (strcmp(field, "Education.High School or Higher") == 0) return entry->education_highschool;
    if (strcmp(field, "Ethnicities.American Indian and Alaska Native Alone") == 0) return entry->ethnicities_indian_alaska;
    if (strcmp(field, "Ethnicities.Asian Alone") == 0) return entry->ethnicities_asian;
    if (strcmp(field, "Ethnicities.Black Alone") == 0) return entry->ethnicities_black;
    if (strcmp(field, "Ethnicities.Hispanic or Latino") == 0) return entry->ethnicities_hispanic;
    if (strcmp(field, "Ethnicities.Native Hawaiian and Other Pacific Islander Alone") == 0) return entry->ethnicities_pacific;
    if (strcmp(field, "Ethnicities.Two or More Races") == 0) return entry->ethnicities_two_or_more;
    if (strcmp(field, "Ethnicities.White Alone") == 0) return entry->ethnicities_white;
    if (strcmp(field, "Ethnicities.White Alone not Hispanic or Latino") == 0) return entry->ethnicities_white_not_hispanic;
    if (strcmp(field, "Income.Persons Below Poverty Level") == 0) return entry->persons_below_poverty;
    return -1;
}

int is_valid_population_field(const char* field) {
return (strncmp(field, "Education.", 10) == 0 ||
strncmp(field, "Ethnicities.", 12) == 0 ||
strcmp(field, "Income.Persons Below Poverty Level") == 0);
}

void operation_display(Demographic* data, int num_records) {
print_demo_data(data, num_records);
}

int operation_filter_state(Demographic* data, int num_records, const char* state) {
int new_count = 0;
    for (int i = 0; i < num_records; i++) {
        if (strcasecmp(data[i].state, state) == 0) {
            if (i != new_count) {
                data[new_count] = data[i];
            }
            new_count++;
        }
    }
    printf("Filter: state == %s (%d entries)\n", state, new_count);
    return new_count;
}

int operation_filter(Demographic* data, int num_records, const char* field, const char* comparison, float value) {
int new_count = 0;
    int is_ge = strcmp(comparison, "ge") == 0;
    
    for (int i = 0; i < num_records; i++) {
        float field_value = get_field_value(&data[i], field);
        if (field_value == -1) continue; // Skip invalid fields
        
        int keep = is_ge ? (field_value >= value) : (field_value <= value);
        if (keep) {
            if (i != new_count) {
                data[new_count] = data[i];
            }
            new_count++;
        }
    }
    
    printf("Filter: %s %s %.2f (%d entries)\n", field, comparison, value, new_count);
    return new_count;
}

void operation_population_total(const Demographic* data, int num_records) {
long long total = 0;
for (int i = 0; i < num_records; i++) {
total += data[i].population_2014;
    }
    printf("2014 population: %lld\n", total);
}

void operation_population_field(const Demographic* data, int num_records, const char* field) {
if (!is_valid_population_field(field)) {
        fprintf(stderr, "Error: Invalid field for population calculation: %s\n", field);
        return;
}
    
    long long total = 0;
    for (int i = 0; i < num_records; i++) {
        float percentage = get_field_value(&data[i], field);
        if (percentage != -1) {
            total += (long long)(data[i].population_2014 * percentage / 100.0);
        }
    }
    printf("2014 %s population: %lld\n", field, total);
}

void operation_percent_field(const Demographic* data, int num_records, const char* field) {
if (!is_valid_population_field(field)) {
        fprintf(stderr, "Error: Invalid field for percentage calculation: %s\n", field);
        return;
    }
    
    double total_population = 0.0;
    double field_population = 0.0;
    
    for (int i = 0; i < num_records; i++) {
        total_population += data[i].population_2014;
        double field_value = get_field_value(&data[i], field);
        if (field_value >= 0.0) {
            field_population += data[i].population_2014 * field_value / 100.0;
        }
    }
    
    double overall_percentage = (total_population > 0) ? (100.0 * field_population / total_population) : 0.0;
    
    printf("2014 %s percentage: %.2f\n", field, overall_percentage);

}

void process_operation(char* line, Demographic* data, int* num_records, int line_number) {
if (strlen(line) == 0) return;
    
    line[strcspn(line, "\n")] = 0;
    
    char* operation = strtok(line, OPERATION_DELIM);
    if (!operation) {
        fprintf(stderr, "Error: Malformed operation at line %d\n", line_number);
        return;
    }

    if (strcmp(operation, "display") == 0) {
        operation_display(data, *num_records);
    }
    else if (strcmp(operation, "filter-state") == 0) {
        char* state = strtok(NULL, OPERATION_DELIM);
        if (!state) {
            fprintf(stderr, "Error: Missing state argument at line %d\n", line_number);
            return;
        }
        *num_records = operation_filter_state(data, *num_records, state);
    }
    else if (strcmp(operation, "filter") == 0) {
        char* field = strtok(NULL, OPERATION_DELIM);
        char* comparison = strtok(NULL, OPERATION_DELIM);
        char* value_str = strtok(NULL, OPERATION_DELIM);
        
        if (!field || !comparison || !value_str) {
            fprintf(stderr, "Error: Missing filter arguments at line %d\n", line_number);
            return;
        }
        
        float value = atof(value_str);
        if (strcmp(comparison, "ge") != 0 && strcmp(comparison, "le") != 0) {
            fprintf(stderr, "Error: Invalid comparison operator at line %d\n", line_number);
            return;
        }
        
        *num_records = operation_filter(data, *num_records, field, comparison, value);
    }
    else if (strcmp(operation, "population-total") == 0) {
        operation_population_total(data, *num_records);
    }
    else if (strcmp(operation, "population") == 0) {
        char* field = strtok(NULL, OPERATION_DELIM);
        if (!field) {
            fprintf(stderr, "Error: Missing field argument at line %d\n", line_number);
            return;
        }
        operation_population_field(data, *num_records, field);
    }
    else if (strcmp(operation, "percent") == 0) {
        char* field = strtok(NULL, OPERATION_DELIM);
        if (!field) {
            fprintf(stderr, "Error: Missing field argument at line %d\n", line_number);
            return;
        }
        operation_percent_field(data, *num_records, field);
    }
    else {
        fprintf(stderr, "Error: Unknown operation '%s' at line %d\n", operation, line_number);
    }
}

void process_operations_file(FILE* ops_file, Demographic* data, int* num_records) {
char line[MAX_LINE_LENGTH];
    int line_number = 0;
    
    while (fgets(line, sizeof(line), ops_file)) {
        line_number++;
        process_operation(line, data, num_records, line_number);
    }
}


int main(int argc, char *argv[]) {

    FILE *demo_file = open_file("county_demographics.csv");
    FILE *ops_file = open_file("filter_state.txt");

    Demographic data[MAX_ENTRIES];
    int num_records = load_demo_data(demo_file, data, MAX_ENTRIES);
    printf("Successfully loaded %d records\n", num_records);

    // print_demo_data(data, num_records);
    process_operations_file(ops_file, data, &num_records);

    fclose(demo_file);
    fclose(ops_file);

    return 0;
}