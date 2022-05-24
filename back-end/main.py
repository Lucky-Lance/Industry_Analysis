from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from pandas import DataFrame
from pydantic import BaseModel

from typing import Union, List, Mapping

app = FastAPI()
# cross-origin resource sharing
app.add_middleware(CORSMiddleware, allow_origins=["*"], allow_methods=["*"],
                   allow_headers=["*"],)

class Dataframe(BaseModel):
    name: str
    content: Union[List[int], None] = None

mini_database: Mapping[int, Dataframe] = {}

@app.get("/")
def read_root():
    return {"Hello": "World"}

@app.get("/items/{id}")
def read_data(id: int):
    d = DataFrame()
    if id in mini_database:
        d = mini_database[id]
        return {"id": id, "name": d.name, "content": d.content}
    else:
        return {"id": "N/A", "name": "default", "content": []}

@app.put("/items/{id}")
def update_data(id: int, frame: Dataframe):
    mini_database[id] = frame
    return { "id": id, "name": frame.name}
