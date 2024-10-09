#include "pch.h"
#include "NOCImport.h"
#include <BRepMesh_DiscretFactory.hxx>
#include <XSControl_Reader.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <STEPControl_Reader.hxx>
#include <IGESControl_Reader.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <StlAPI_Writer.hxx>

NOCImport::NOCImport()
{
}

bool NOCImport::ConvExt2STLb(XSControl_Reader& Reader, const char* SourceFile, const char* DestFile, bool ASCII, double Tol, double Angle)
{
    IFSelect_ReturnStatus status = Reader.ReadFile(SourceFile);
    if (status != IFSelect_RetDone)
        return false;

    //Reader.WS()->TransferReader()->TransientProcess()->SetTraceLevel(2); // increase default trace level
    //Standard_Boolean failsonly = Standard_False;
    //Reader.PrintCheckLoad(failsonly, IFSelect_ItemsByEntity);
    Handle(TopTools_HSequenceOfShape) aHSequenceOfShape = new TopTools_HSequenceOfShape();
    if (dynamic_cast<STEPControl_Reader*>(&Reader) != nullptr)
    {
        // IGES import
        // Root transfers
        Standard_Integer nbr = Reader.NbRootsForTransfer();
        //Reader.PrintCheckTransfer(failsonly, IFSelect_ItemsByEntity);
        for (Standard_Integer n = 1; n <= nbr; n++)
            Reader.TransferOneRoot(n);

        // Collecting resulting entities
        Standard_Integer nbs = Reader.NbShapes();
        if (nbs == 0)
            return false;
        for (Standard_Integer i = 1; i <= nbs; i++)
            aHSequenceOfShape->Append(Reader.Shape(i));
    }
    else if (dynamic_cast<IGESControl_Reader*>(&Reader) != nullptr)
    {
        // IGES import
        Reader.TransferRoots();
        TopoDS_Shape aShape = Reader.OneShape();
        aHSequenceOfShape->Append(aShape);
    }

    // STL export
    if (aHSequenceOfShape->Length() == 0)
        return false;

    TopoDS_Compound RES;
    BRep_Builder MKCP;
    MKCP.MakeCompound(RES);

    for (Standard_Integer i = 1; i <= aHSequenceOfShape->Length(); i++)
    {
        TopoDS_Shape aShape = aHSequenceOfShape->Value(i);

        if (aShape.IsNull())
        {
            status = IFSelect_RetFail;
            continue;
        }
        BRepTools::Clean(aShape);
        // retrieve meshing tool from Factory
        Handle(BRepMesh_DiscretRoot) aMeshAlgo = BRepMesh_DiscretFactory::Get().Discret(aShape,
            Tol,
            Angle);
        if (!aMeshAlgo.IsNull())
        {
            aMeshAlgo->Perform();
        }

        MKCP.Add(RES, aShape);
    }

    StlAPI_Writer myStlWriter;
    myStlWriter.ASCIIMode() = ASCII;
    if (Standard_False == myStlWriter.Write(RES, DestFile))
        return false;
    return true;
}

bool NOCImport::ConvExt2STL(StockFileType StockType, const char* SourceFile, const char* DestFile, bool ASCII, double Tol, double Angle)
{
 
    int SourceLen = int(strnlen_s(SourceFile, 32000));
    int len = MultiByteToWideChar(CP_THREAD_ACP, 0, SourceFile, SourceLen, NULL, 0);
    LPWSTR UCSourceFile = new wchar_t[len + 1];
    MultiByteToWideChar(CP_THREAD_ACP, 0, SourceFile, SourceLen, UCSourceFile, len);
    UCSourceFile[len] = '\x0';
    TCollection_AsciiString SFileName(UCSourceFile);

    SourceLen = int(strnlen_s(DestFile, 32000));
    len = MultiByteToWideChar(CP_THREAD_ACP, 0, DestFile, SourceLen, NULL, 0);
    LPWSTR UCDestFile = new wchar_t[len + 1];
    MultiByteToWideChar(CP_THREAD_ACP, 0, DestFile, SourceLen, UCDestFile, len);
    UCDestFile[len] = '\x0';
    TCollection_AsciiString DFileName(UCDestFile);

    switch (StockType)
    {
    case SFT_STEP:
        {
            STEPControl_Reader Reader;
            return ConvExt2STLb(Reader, SFileName.ToCString(), DFileName.ToCString(), ASCII, Tol, Angle);
        }
    case SFT_IGES:
        {
            IGESControl_Reader Reader;
            return ConvExt2STLb(Reader, SFileName.ToCString(), DFileName.ToCString(), ASCII, Tol, Angle);
        }
    }
    return false;
}
