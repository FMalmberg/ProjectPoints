#include "c4d.h"
#include "c4d_symbols.h"

class ProjectPoints : public CommandData
{
	public:
		virtual Bool Execute(BaseDocument *doc, GeDialog *parentManager);
};
  

Bool ProjectPoints::Execute(BaseDocument *doc, GeDialog *parentManager)
{
	BaseObject* obj = doc->GetActiveObject();
	if(obj==NULL) return TRUE;

	if (!obj->IsInstanceOf(Opoint)) return TRUE;

	Vector* padr = ToPoint(obj)->GetPointW();	
	long pcnt = ToPoint(obj)->GetPointCount(); if (!pcnt) return TRUE;

	BaseSelect* bs = ToPoint(obj)->GetPointS();
	
	Int32 seg=0,a,b,i;

	SplineObject* spline = SplineObject::Alloc(bs->GetCount(),SPLINETYPE::LINEAR);
	
	Matrix m=obj->GetMgn();
	Matrix im=~m;

	Vector* SplinePoints= ToPoint(spline)->GetPointW();

	int j=0;
	while (bs->GetRange(seg++, LIMIT<Int32>::MAX, &a,&b)){
		for (i=a; i<=b; ++i){
			SplinePoints[j]=m*padr[i];
			j++;
		}
	}

	
	BaseObject* pred = obj->GetPred();
	BaseObject* parent = obj->GetUp();
	obj->Remove();

	BaseContainer bc;
	bc.SetInt32(MDATA_SPLINE_PROJECTMODE, 0);
	ModelingCommandData cd;
	cd.doc = doc;
	cd.bc = &bc;
	cd.op = spline;
	if (!SendModelingCommand(MCOMMAND_SPLINE_PROJECT, cd)) return TRUE;

	doc->InsertObject(obj,parent,pred);
	
	j=0;
	seg=0;
	
	doc->StartUndo();
	doc->AddUndo(UNDOTYPE::CHANGE, obj);

	while (bs->GetRange(seg++, LIMIT<Int32>::MAX,&a,&b)){
		for (i=a; i<=b; ++i){
			padr[i]=im*SplinePoints[j];
			j++;
		}
	}

	doc->EndUndo();	
	SplineObject::Free(spline);

	obj->SetDirty(DIRTYFLAGS::ALL);

	EventAdd();

	return TRUE;
}

Bool RegisterProjectPoints(void)
{
	long ID=1022067;
	return RegisterCommandPlugin(ID,String("Project Points"),0, AutoBitmap(String("ProjectPoints.tif")),String("Project point selection onto other surfaces"),NewObjClear(ProjectPoints) );
}