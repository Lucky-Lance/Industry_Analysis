from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from pandas import DataFrame
from pydantic import BaseModel
import json, uvicorn

from typing import Union, List, Mapping

app = FastAPI()
# cross-origin resource sharing
app.add_middleware(CORSMiddleware, allow_origins=["*"], allow_methods=["*"],
                   allow_headers=["*"],)

mainGraphData = json.load(open("./data/mainGraphData.json", "r"))
subGraphData = json.load(open("./data/subGraphData.json", "r"))
tableData = json.load(open("./data/tableData.json", "r"))
rightDownCharData = json.load(open("./data/rightDownCharData.json", "r"))


@app.get("/")
def read_root():
    return {"Hello": "World"}

@app.get("/mainGraphData")
def read_mainGraphData():
    return mainGraphData
    
@app.get("/subGraphData/{id}")
def read_subGraphData(id: int):
    return subGraphData[str(id)]

@app.get("/tableData/{id}")
def read_tableData(id: int):
    return tableData[str(id)]

@app.get("/rightDownCharData/{id}")
def read_rightDownCharData(id: int):
    return rightDownCharData[str(id)]


if __name__ == "__main__":
    uvicorn.run("main:app", host="192.168.1.107", port=8000)