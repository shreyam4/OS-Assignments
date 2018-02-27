#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<unistd.h>
#include<wait.h>
void selectionSort(int arr[], int n)
{
	int i, key, j, min_ind,temp;
	for(i=0; i<n; i++)
	{
		min_ind = i;
		for(j=i; j<n; j++)
		{
			if(arr[min_ind] > arr[j])
			{
				min_ind = j;
			}
		}
		temp = arr[i];
		arr[i]  =arr[min_ind];
		arr[min_ind] = temp;
	}
}

void merge(int a[], int f, int mid, int l)
{
	int count = l-f+1;
	int sorted[count];
	int i=f, k=mid+1, m=0;
	while(i<=mid && k<=l)
	{
		if (a[i]<a[k])
			sorted[m++]=a[i++];
		else if(a[k]<a[i])
			sorted[m++]=a[k++];
		else if(a[i]==a[k])
		{
			sorted[m++]=a[i++];
			sorted[m++]=a[k++];
		}
	}
	while(i<=mid)
		sorted[m++]=a[i++];
	while(k<=l)
		sorted[m++]=a[k++];
	int arr_count = f;
	for (i=0; i<count; i++,f++)
		a[f] = sorted[i];

}
void mergeSort(int a[], int f, int l)
{
	int i, len=(l-f+1);
	
	if(len<=5)
	{
		selectionSort(a+f, len);
		return;
	}
	pid_t lpid,rpid;
	lpid = fork();
	if(lpid<0)
	{
		perror("left child not created");
		exit(-1);
	}
	else if (lpid == 0)
	{
		mergeSort(a,f,f+(len/2)-1);
		exit(0);
	}
	else
	{
		rpid = fork();
		if(rpid<0)
		{
			perror("right child not created");
			exit(-1);
		}
		else if(rpid==0)
		{
			mergeSort(a, f+len/2, l);
			exit(0);
		}
	}
	int status;
	waitpid(lpid, &status, 0);
	waitpid(rpid, &status, 0);

	merge(a, f, f+(len/2)-1, l);
}


int main()
{
	int shmid;
	key_t key = IPC_PRIVATE;
	int n,i;
	int *arr;
	scanf("%d", &n);
	size_t SHM_SIZE = sizeof(int)*n;

	if((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0)
	{
		perror("shmget");
		exit(1);
	}
	if((arr = shmat(shmid, NULL, 0)) == (int *) -1)
	{
		perror("shmat");
		exit(1);
	}

	for(i=0; i<n; i++)
	{
		scanf("%d", &arr[i]);
	}
	mergeSort(arr, 0, n-1);
//	selectionSort(arr,n);
	for(i=0; i<n; i++)
	{
		printf("%d ",arr[i]);
	}
	printf("\n");
	if(shmdt(arr) == -1)
	{
		perror("shmdt");
		exit(1);
	}
	if(shmctl(shmid, IPC_RMID, NULL) == -1)
	{
		perror("shmctl");
		exit(1);
	}
	return 0;
}


/*Concurrent merge Sort vs. Normal merge Sort

The performance of normal merge sort is comparatively better than the concurrent merge sort. When the left child accesses the left array, the array is loaded into the cache of a processor.Next, when the right array is accessed,there is a cache miss because the cache is filled with left segment and so the right segment is copied to the cache memory. This process is continued which leads to such degradation of performance that it performs poorer than the normal merge sort.
*/

	
